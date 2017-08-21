
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


/**
*  @brief
*    Clear buffer with generic clear value
*
*  @param[in] fbo
*    Framebuffer object
*  @param[in] type
*    Attachment type
*  @param[in] type
*    Buffer to clear
*  @param[in] value
*    Clear value
*/
template <typename T>
void clearBuffer(globjects::Framebuffer * fbo, gl::GLenum type, gl::GLint drawBuffer, T value)
{
    fbo->clearBuffer(type, drawBuffer, value);
}

/**
*  @brief
*    Clear buffer with color value
*
*  @param[in] fbo
*    Framebuffer object
*  @param[in] type
*    Attachment type
*  @param[in] type
*    Buffer to clear
*  @param[in] value
*    Clear value
*/
template <>
void clearBuffer(globjects::Framebuffer * fbo, gl::GLenum type, gl::GLint drawBuffer, gloperate::Color value)
{
    fbo->clearBuffer(type, drawBuffer, value.toVec4());
}

/**
*  @brief
*    Clear buffer with depth and stencil value
*
*  @param[in] fbo
*    Framebuffer object
*  @param[in] type
*    Attachment type
*  @param[in] type
*    Buffer to clear
*  @param[in] value
*    Clear value
*/
template <>
void clearBuffer(globjects::Framebuffer * fbo, gl::GLenum type, gl::GLint drawBuffer, std::pair<float, int> value)
{
    fbo->clearBuffer(type, value.first, value.second, drawBuffer);
}


} // namespace


namespace gloperate
{


CPPEXPOSE_COMPONENT(ClearStage, gloperate::Stage)


/**
*  @brief
*    Base class for clear values
*/
class AbstractClearInput
{
public:
    AbstractClearInput()
    {
    }

    virtual ~AbstractClearInput()
    {
    }

    virtual void clear(globjects::Framebuffer * fbo, gl::GLint drawBuffer) const = 0;
    virtual bool isComplete() const = 0;
    virtual const Input<AbstractRenderTarget *> * renderTargetInput() const = 0;
};


/**
*  @brief
*    Representation of a clear value
*
*    A clear value corresponds to a render target input and its
*    respective clear value input.
*/
template <typename T>
class ClearValueInput : public AbstractClearInput
{
public:
    ClearValueInput(const Input<T> * clearValueInput, const Input<AbstractRenderTarget *> * renderTargetInput)
    : m_renderTargetInput(renderTargetInput)
    , m_clearValueInput(clearValueInput)
    {
    }

    virtual void clear(globjects::Framebuffer * fbo, gl::GLint drawBuffer) const override
    {
        assert(isComplete());

        clearBuffer(fbo, (**m_renderTargetInput)->attachmentGLType(), (**m_renderTargetInput)->clearBufferDrawBuffer(drawBuffer), m_clearValueInput->value());
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
    const Input<AbstractRenderTarget *> * m_renderTargetInput; ///< Input that contains the render target
    const Input<T>                      * m_clearValueInput;   ///< Input that contains the clear value
};


/**
*  @brief
*    Helper class to add a clear value of a specific type
*/
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
    void invoke()
    {
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
    // Reconfigure clear stage whenever a new input has been added
    inputAdded.connect([this] (AbstractSlot *)
    {
        m_reprocessInputs = true;
    });
}

ClearStage::~ClearStage()
{
}

void ClearStage::onContextInit(AbstractGLContext *)
{
    // Initialize render targets
    renderInterface.onContextInit();
}

void ClearStage::onContextDeinit(AbstractGLContext *)
{
    // De-initialize render targets
    renderInterface.onContextDeinit();
}

void ClearStage::onProcess()
{
    // Reconfigure clear stage if scheduled
    if (m_reprocessInputs)
    {
        reprocessInputs();

        m_reprocessInputs = false;
    }

    // Check if clearing is enabled
    if (*clear)
    {
        // Initialize state
        bool scissorEnabled = false;

        // Determine if scissor is enabled
        if (renderInterface.viewport->z >= 0.0 || renderInterface.viewport->w >= 0.0)
        {
            // Setup OpenGL state
            gl::glScissor(renderInterface.viewport->x, renderInterface.viewport->y, renderInterface.viewport->z, renderInterface.viewport->w);
            gl::glEnable(gl::GL_SCISSOR_TEST);

            // Scissor is enabled
            scissorEnabled = true;
        }
        else
        {
            // Clear full render targets if viewport has invalid size
            gl::glDisable(gl::GL_SCISSOR_TEST);
        }

        // Clear all render targets
        size_t colorAttachmentIndex = 0;

        for (const auto & clearValueInput : m_clearInputs)
        {
            // Abort if pair of render target and clear value is invalid
            if (!clearValueInput->isComplete())
            {
                break; // All further clear value inputs won't be complete, either
            }

            // Get render target and obtain FBO for it
            AbstractRenderTarget * renderTarget = **clearValueInput->renderTargetInput();
            auto fbo = renderInterface.obtainFBO(colorAttachmentIndex, renderTarget);

            // Clear render target
            clearValueInput->clear(fbo, colorAttachmentIndex);

            // Count color attachments
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
    // Reset configuration
    m_clearInputs.clear();

    // Call function on each slot, abort if condition is true
    const auto skipUntil = [] (std::vector<AbstractSlot *>::iterator & it, std::vector<AbstractSlot *>::const_iterator end, std::function<bool(AbstractSlot *)> callback)
    {
        do
        {
            ++it;
        }
        while (it != end && !callback(*it));
    };

    // Iterate over all inputs to match render targets with corresponing clear values
    auto clearValueIt = m_inputs.begin();
    auto renderTargetIt = m_inputs.begin();

    while (clearValueIt != m_inputs.end() && renderTargetIt != m_inputs.end())
    {
        // Find next input that defines a clear value
        skipUntil(clearValueIt, m_inputs.end(), [] (AbstractSlot * input)
        {
            return input->isOfAnyType<Color, float, int, std::pair<float, int>>();
        });

        // Find next input that defines a render target
        skipUntil(renderTargetIt, m_inputs.end(), [] (AbstractSlot * input)
        {
            return input->isOfAnyType<ColorRenderTarget *, DepthRenderTarget *, StencilRenderTarget *, DepthStencilRenderTarget *>();
        });

        // Abort if no further match has been found
        if (clearValueIt == m_inputs.end() || renderTargetIt == m_inputs.end())
        {
            break;
        }

        // Get render target and clear value inputs
        auto renderTargetInput = reinterpret_cast<Input<AbstractRenderTarget *> *>(*renderTargetIt);
        auto clearValueInput   = *clearValueIt;

        // Add clear value
        SupportedClearValueTypes::apply(ClearValueAdder(this, clearValueInput, renderTargetInput));
    }
}


} // namespace gloperate
