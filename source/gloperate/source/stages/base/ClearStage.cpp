
#include <gloperate/stages/base/ClearStage.h>

#include <algorithm>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedRenderbuffer.h>
#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/StencilRenderTarget.h>


namespace
{


template <typename T>
void clearBuffer(globjects::Framebuffer * fbo, gl::GLenum type, gl::GLint drawBuffer, const gloperate::Input<T> * input)
{
    fbo->clearBuffer(type, drawBuffer, input->value());
}

template <>
void clearBuffer(globjects::Framebuffer * fbo, gl::GLenum type, gl::GLint drawBuffer, const gloperate::Input<gloperate::Color> * input)
{
    const auto & color = input->value();

    fbo->clearBuffer(type, drawBuffer, color.toVec4());
}

template <>
void clearBuffer(globjects::Framebuffer * fbo, gl::GLenum type, gl::GLint drawBuffer, const gloperate::Input<std::pair<float, int>> * input)
{
    const auto & pair = input->value();

    fbo->clearBuffer(type, pair.first, pair.second, drawBuffer);
}


} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(ClearStage, gloperate::Stage)


class AbstractClearInput
{
public:
    virtual ~AbstractClearInput()
    {
    }

    virtual void clear(globjects::Framebuffer * fbo, gl::GLint drawBuffer) const = 0;

    virtual bool isComplete() const = 0;

    virtual const Input<AbstractRenderTarget *> * renderTargetInput() const = 0;
};


template <typename T>
class ClearValueInput : public AbstractClearInput
{
public:
    ClearValueInput(const Input<T> * clearValueInput, const Input<AbstractRenderTarget *> * renderTargetInput)
    : m_clearValueInput(clearValueInput)
    , m_renderTargetInput(renderTargetInput)
    {
    }

    virtual void clear(globjects::Framebuffer * fbo, gl::GLint drawBuffer) const override
    {
        assert(isComplete());

        clearBuffer(fbo, (**m_renderTargetInput)->attachmentGLType(), (**m_renderTargetInput)->clearBufferDrawBuffer(drawBuffer), m_clearValueInput);
    }

    virtual bool isComplete() const override
    {
        return m_clearValueInput != nullptr && m_renderTargetInput != nullptr;
    }

    virtual const Input<AbstractRenderTarget *> * renderTargetInput() const override
    {
        return m_renderTargetInput;
    }

protected:
    const Input<T> * m_clearValueInput;
    const Input<AbstractRenderTarget *> * m_renderTargetInput;
};


class ClearValueAdder
{
public:
    ClearValueAdder(ClearStage * stage, AbstractSlot * clearValueInput, Input<AbstractRenderTarget *> * renderTargetInput)
    : m_stage(stage)
    , m_clearValueInput(clearValueInput)
    , m_renderTargetInput(renderTargetInput)
    {
    }

    template< typename T>
    void operator()() {
        auto clearValueInputT = dynamic_cast<const Input<T> *>(m_clearValueInput);

        if (clearValueInputT)
        {
            m_stage->m_clearInputs.emplace_back(new ClearValueInput<T>(clearValueInputT, m_renderTargetInput));
        }
    }


protected:
          ClearStage                    * m_stage;
    const AbstractSlot                  * m_clearValueInput;
    const Input<AbstractRenderTarget *> * m_renderTargetInput;
};


ClearStage::ClearStage(Environment * environment, const std::string & name)
: Stage(environment, "ClearStage", name)
, renderInterface(this)
, clear("clear",  this, true)
, m_reprocessInputs(false)
{
    inputAdded.connect([this] (AbstractSlot * /*connectedInput*/) {
        m_reprocessInputs = true;
    });
}

ClearStage::~ClearStage()
{
}

void ClearStage::onContextInit(AbstractGLContext *)
{
    renderInterface.onContextInit();
}

void ClearStage::onContextDeinit(AbstractGLContext *)
{
    renderInterface.onContextDeinit();
}

void ClearStage::onProcess()
{
    if (m_reprocessInputs)
    {
        reprocessInputs();

        m_reprocessInputs = false;
    }

    if (*clear)
    {
        bool scissorEnabled = false;

        if (renderInterface.viewport->z >= 0.0 || renderInterface.viewport->w >= 0.0) {
            // Setup OpenGL state
            gl::glScissor(renderInterface.viewport->x, renderInterface.viewport->y, renderInterface.viewport->z, renderInterface.viewport->w);
            gl::glEnable(gl::GL_SCISSOR_TEST);

            scissorEnabled = true;
        }
        else
        {
            // Clear full render targets if viewport has invalid size
            gl::glDisable(gl::GL_SCISSOR_TEST);
        }

        size_t colorAttachmentIndex = 0;

        for (const auto & clearValueInput : m_clearInputs)
        {
            if (!clearValueInput->isComplete())
            {
                break; // All further clear value inputs won't be complete, either
            }

            AbstractRenderTarget * renderTarget = **clearValueInput->renderTargetInput();

            auto fbo = renderInterface.obtainFBO(colorAttachmentIndex, renderTarget);

            clearValueInput->clear(fbo, colorAttachmentIndex);

            if (renderTarget->underlyingAttachmentType() == AttachmentType::Color)
            {
                ++colorAttachmentIndex;
            }
        }

        // Reset OpenGL state
        if (scissorEnabled)
        {
            gl::glDisable(gl::GL_SCISSOR_TEST);
        }
    }

    // Update outputs
    renderInterface.updateRenderTargetOutputs();
}

void ClearStage::reprocessInputs()
{
    m_clearInputs.clear();

    auto clearValueIt = m_inputs.begin();
    auto renderTargetIt = m_inputs.begin();

    const auto skipUntil = [](std::vector<AbstractSlot *>::iterator & it, std::vector<AbstractSlot *>::const_iterator end, std::function<bool(AbstractSlot *)> callback) {
        do
        {
            ++it;
        }
        while (it != end && !callback(*it));
    };

    while (clearValueIt != m_inputs.end() && renderTargetIt != m_inputs.end()) // while(true)? do...while(...)?
    {
        skipUntil(clearValueIt, m_inputs.end(), [](AbstractSlot * input) {
            return input->isOfAnyType<Color, float, int, std::pair<float, int>>();
        });
        skipUntil(renderTargetIt, m_inputs.end(), [](AbstractSlot * input) {
            return input->isOfAnyType<ColorRenderTarget *, DepthRenderTarget *, StencilRenderTarget *, DepthStencilRenderTarget *>();
        });

        if (clearValueIt == m_inputs.end() || renderTargetIt == m_inputs.end())
        {
            break;
        }

        auto renderTargetInput = reinterpret_cast<Input<AbstractRenderTarget *> *>(*renderTargetIt);

        SupportedClearValueTypes::apply(ClearValueAdder(this, *clearValueIt, renderTargetInput));
    }
}


} // namespace gloperate
