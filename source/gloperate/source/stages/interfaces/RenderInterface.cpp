
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <cmath>

#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedRenderbuffer.h>
#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/RenderTargetType.h>
#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/DepthStencilRenderTarget.h>
#include <gloperate/rendering/StencilRenderTarget.h>


namespace gloperate
{


RenderInterface::RenderInterface(Stage * stage)
: viewport("viewport", stage, glm::vec4(0.0, 0.0, -1.0, -1.0))
{
    // Hide inputs in property editor
    viewport.setOption("hidden", true);

    stage->inputAdded.connect( [this] (AbstractSlot * connectedInput) {
        auto colorRenderTargetInput = dynamic_cast<Input<ColorRenderTarget *> *>(connectedInput);
        auto depthRenderTargetInput = dynamic_cast<Input<DepthRenderTarget *> *>(connectedInput);
        auto depthStencilRenderTargetInput = dynamic_cast<Input<DepthStencilRenderTarget *> *>(connectedInput);
        auto stencilRenderTargetInput = dynamic_cast<Input<StencilRenderTarget *> *>(connectedInput);

        if (colorRenderTargetInput)
        {
            addRenderTargetInput(colorRenderTargetInput);
        }

        if (depthRenderTargetInput)
        {
            addRenderTargetInput(depthRenderTargetInput);
        }

        if (depthStencilRenderTargetInput)
        {
            addRenderTargetInput(depthStencilRenderTargetInput);
        }

        if (stencilRenderTargetInput)
        {
            addRenderTargetInput(stencilRenderTargetInput);
        }
    });

    stage->outputAdded.connect( [this] (AbstractSlot * connectedOutput) {
        auto colorRenderTargetOutput = dynamic_cast<Output<ColorRenderTarget *> *>(connectedOutput);
        auto depthRenderTargetOutput = dynamic_cast<Output<DepthRenderTarget *> *>(connectedOutput);
        auto depthStencilRenderTargetOutput = dynamic_cast<Output<DepthStencilRenderTarget *> *>(connectedOutput);
        auto stencilRenderTargetOutput = dynamic_cast<Output<StencilRenderTarget *> *>(connectedOutput);

        if (colorRenderTargetOutput)
        {
            addRenderTargetOutput(colorRenderTargetOutput);
        }

        if (depthRenderTargetOutput)
        {
            addRenderTargetOutput(depthRenderTargetOutput);
        }

        if (depthStencilRenderTargetOutput)
        {
            addRenderTargetOutput(depthStencilRenderTargetOutput);
        }

        if (stencilRenderTargetOutput)
        {
            addRenderTargetOutput(stencilRenderTargetOutput);
        }
    });
}

RenderInterface::~RenderInterface()
{
}

void RenderInterface::updateRenderTargetOutputs()
{
    pairwiseColorRenderTargetsDo([](Input<ColorRenderTarget *> * input, Output<ColorRenderTarget *> * output) {
        output->setValue(**input);
    });
    pairwiseDepthRenderTargetsDo([](Input<DepthRenderTarget *> * input, Output<DepthRenderTarget *> * output) {
        output->setValue(**input);
    });
    pairwiseDepthStencilRenderTargetsDo([](Input<DepthStencilRenderTarget *> * input, Output<DepthStencilRenderTarget *> * output) {
        output->setValue(**input);
    });
    pairwiseStencilRenderTargetsDo([](Input<StencilRenderTarget *> * input, Output<StencilRenderTarget *> * output) {
        output->setValue(**input);
    });
}

bool RenderInterface::allRenderTargetsCompatible() const
{
    if (m_colorRenderTargetInputs.empty() && m_depthRenderTargetInputs.empty() && m_depthStencilRenderTargetInputs.empty() && m_stencilRenderTargetInputs.empty())
    {
        return true;
    }

    auto numberOfDepthAttachments = m_depthRenderTargetOutputs.size() + m_depthStencilRenderTargetOutputs.size();
    auto numberOfStencilAttachments = m_depthStencilRenderTargetOutputs.size() + m_stencilRenderTargetOutputs.size();

    auto allDefaultFramebufferAttachments =
        std::all_of(m_colorRenderTargetInputs.begin(), m_colorRenderTargetInputs.end(), [](Input<ColorRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })
        && std::all_of(m_depthRenderTargetInputs.begin(), m_depthRenderTargetInputs.end(), [](Input<DepthRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })
        && std::all_of(m_depthStencilRenderTargetInputs.begin(), m_depthStencilRenderTargetInputs.end(), [](Input<DepthStencilRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })
        && std::all_of(m_stencilRenderTargetInputs.begin(), m_stencilRenderTargetInputs.end(), [](Input<StencilRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        });

    auto allUserDefinedFramebufferAttachments = std::all_of(m_colorRenderTargetInputs.begin(), m_colorRenderTargetInputs.end(), [](Input<ColorRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })
        && std::all_of(m_depthRenderTargetInputs.begin(), m_depthRenderTargetInputs.end(), [](Input<DepthRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })
        && std::all_of(m_depthStencilRenderTargetInputs.begin(), m_depthStencilRenderTargetInputs.end(), [](Input<DepthStencilRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })
        && std::all_of(m_stencilRenderTargetInputs.begin(), m_stencilRenderTargetInputs.end(), [](Input<StencilRenderTarget *> * input) {
            if (input == nullptr)
            {
                return true;
            }

            auto renderTarget = **input;

            if (renderTarget == nullptr)
            {
                return true;
            }

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        });

    return allDefaultFramebufferAttachments != allUserDefinedFramebufferAttachments && numberOfDepthAttachments <= 1 && numberOfStencilAttachments <= 1;
}

const std::vector<Input<ColorRenderTarget *> *> & RenderInterface::colorRenderTargetInputs() const
{
    return m_colorRenderTargetInputs;
}

const std::vector<Input<DepthRenderTarget *> *> & RenderInterface::depthRenderTargetInputs() const
{
    return m_depthRenderTargetInputs;
}

const std::vector<Input<DepthStencilRenderTarget *> *> & RenderInterface::depthStencilRenderTargetInputs() const
{
    return m_depthStencilRenderTargetInputs;
}

const std::vector<Input<StencilRenderTarget *> *> & RenderInterface::stencilRenderTargetInputs() const
{
    return m_stencilRenderTargetInputs;
}

Input<ColorRenderTarget *> * RenderInterface::colorRenderTargetInput(size_t index) const
{
    return m_colorRenderTargetInputs.size() > index ? m_colorRenderTargetInputs.at(index) : nullptr;
}

Input<DepthRenderTarget *> * RenderInterface::depthRenderTargetInput(size_t index) const
{
    return m_depthRenderTargetInputs.size() > index ? m_depthRenderTargetInputs.at(index) : nullptr;
}

Input<DepthStencilRenderTarget *> * RenderInterface::depthStencilRenderTargetInput(size_t index) const
{
    return m_depthStencilRenderTargetInputs.size() > index ? m_depthStencilRenderTargetInputs.at(index) : nullptr;
}

Input<StencilRenderTarget *> * RenderInterface::stencilRenderTargetInput(size_t index) const
{
    return m_stencilRenderTargetInputs.size() > index ? m_stencilRenderTargetInputs.at(index) : nullptr;
}

ColorRenderTarget * RenderInterface::colorRenderTarget(size_t index) const
{
    const auto input = colorRenderTargetInput(index);

    return input ? **input : nullptr;
}

DepthRenderTarget * RenderInterface::depthRenderTarget(size_t index) const
{
    const auto input = depthRenderTargetInput(index);

    return input ? **input : nullptr;
}

DepthStencilRenderTarget * RenderInterface::depthStencilRenderTarget(size_t index) const
{
    const auto input = depthStencilRenderTargetInput(index);

    return input ? **input : nullptr;
}

StencilRenderTarget * RenderInterface::stencilRenderTarget(size_t index) const
{
    const auto input = stencilRenderTargetInput(index);

    return input ? **input : nullptr;
}

const std::vector<Output<ColorRenderTarget *> *> & RenderInterface::colorRenderTargetOutputs() const
{
    return m_colorRenderTargetOutputs;
}

const std::vector<Output<DepthRenderTarget *> *> & RenderInterface::depthRenderTargetOutputs() const
{
    return m_depthRenderTargetOutputs;
}

const std::vector<Output<DepthStencilRenderTarget *> *> & RenderInterface::depthStencilRenderTargetOutputs() const
{
    return m_depthStencilRenderTargetOutputs;
}

const std::vector<Output<StencilRenderTarget *> *> & RenderInterface::stencilRenderTargetOutputs() const
{
    return m_stencilRenderTargetOutputs;
}

Output<ColorRenderTarget *> * RenderInterface::colorRenderTargetOutput(size_t index) const
{
    return m_colorRenderTargetOutputs.size() > index ? m_colorRenderTargetOutputs.at(index) : nullptr;
}

Output<DepthRenderTarget *> * RenderInterface::depthRenderTargetOutput(size_t index) const
{
    return m_depthRenderTargetOutputs.size() > index ? m_depthRenderTargetOutputs.at(index) : nullptr;
}

Output<DepthStencilRenderTarget *> * RenderInterface::depthStencilRenderTargetOutput(size_t index) const
{
    return m_depthStencilRenderTargetOutputs.size() > index ? m_depthStencilRenderTargetOutputs.at(index) : nullptr;
}

Output<StencilRenderTarget *> * RenderInterface::stencilRenderTargetOutput(size_t index) const
{
    return m_stencilRenderTargetOutputs.size() > index ? m_stencilRenderTargetOutputs.at(index) : nullptr;
}

void RenderInterface::addRenderTargetInput(Input<ColorRenderTarget *> * input)
{
    m_colorRenderTargetInputs.push_back(input);
}

void RenderInterface::addRenderTargetInput(Input<DepthRenderTarget *> * input)
{
    m_depthRenderTargetInputs.push_back(input);
}

void RenderInterface::addRenderTargetInput(Input<DepthStencilRenderTarget *> * input)
{
    m_depthStencilRenderTargetInputs.push_back(input);
}

void RenderInterface::addRenderTargetInput(Input<StencilRenderTarget *> * input)
{
    m_stencilRenderTargetInputs.push_back(input);
}

void RenderInterface::addRenderTargetOutput(Output<ColorRenderTarget *> * input)
{
    m_colorRenderTargetOutputs.push_back(input);
}

void RenderInterface::addRenderTargetOutput(Output<DepthRenderTarget *> * input)
{
    m_depthRenderTargetOutputs.push_back(input);
}

void RenderInterface::addRenderTargetOutput(Output<DepthStencilRenderTarget *> * input)
{
    m_depthStencilRenderTargetOutputs.push_back(input);
}

void RenderInterface::addRenderTargetOutput(Output<StencilRenderTarget *> * input)
{
    m_stencilRenderTargetOutputs.push_back(input);
}

void RenderInterface::pairwiseColorRenderTargetsDo(std::function<void(Input<ColorRenderTarget *> *, Output<ColorRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_colorRenderTargetInputs.size(), m_colorRenderTargetOutputs.size())
        : std::min(m_colorRenderTargetInputs.size(), m_colorRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(colorRenderTargetInput(i), colorRenderTargetOutput(i));
    }
}

void RenderInterface::pairwiseDepthRenderTargetsDo(std::function<void(Input<DepthRenderTarget *> *, Output<DepthRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_depthRenderTargetInputs.size(), m_depthRenderTargetOutputs.size())
        : std::min(m_depthRenderTargetInputs.size(), m_depthRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(depthRenderTargetInput(i), depthRenderTargetOutput(i));
    }
}

void RenderInterface::pairwiseDepthStencilRenderTargetsDo(std::function<void(Input<DepthStencilRenderTarget *> *, Output<DepthStencilRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_depthStencilRenderTargetInputs.size(), m_depthStencilRenderTargetOutputs.size())
        : std::min(m_depthStencilRenderTargetInputs.size(), m_depthStencilRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(depthStencilRenderTargetInput(i), depthStencilRenderTargetOutput(i));
    }
}

void RenderInterface::pairwiseStencilRenderTargetsDo(std::function<void(Input<StencilRenderTarget *> *, Output<StencilRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_stencilRenderTargetInputs.size(), m_stencilRenderTargetOutputs.size())
        : std::min(m_stencilRenderTargetInputs.size(), m_stencilRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(stencilRenderTargetInput(i), stencilRenderTargetOutput(i));
    }
}

globjects::Framebuffer * RenderInterface::obtainFBO() const
{
    assert(allRenderTargetsCompatible());

    std::vector<gl::GLenum> drawBuffers;
    globjects::Framebuffer * currentFBO = nullptr;
    auto colorAttachmentIndex = size_t(0);
    for (auto input : m_colorRenderTargetInputs)
    {
        auto nextFBO = obtainFBO(colorAttachmentIndex, **input);

        if (!currentFBO)
        {
            currentFBO = nextFBO;
        }

        if (nextFBO != currentFBO)
        {
            return nullptr;
        }

        if (**input)
        {
            drawBuffers.push_back((**input)->drawBufferAttachment(colorAttachmentIndex));
        }
        else
        {
            drawBuffers.push_back(gl::GL_NONE);
        }

        ++colorAttachmentIndex;
    }

    for (auto input : m_depthRenderTargetInputs)
    {
        auto nextFBO = obtainFBO(0, **input);

        if (!currentFBO)
        {
            currentFBO = nextFBO;
        }

        if (nextFBO != currentFBO)
        {
            return nullptr;
        }
    }

    for (auto input : m_depthStencilRenderTargetInputs)
    {
        auto nextFBO = obtainFBO(0, **input);

        if (!currentFBO)
        {
            currentFBO = nextFBO;
        }

        if (nextFBO != currentFBO)
        {
            return nullptr;
        }
    }

    for (auto input : m_stencilRenderTargetInputs)
    {
        auto nextFBO = obtainFBO(0, **input);

        if (!currentFBO)
        {
            currentFBO = nextFBO;
        }

        if (nextFBO != currentFBO)
        {
            return nullptr;
        }
    }

    currentFBO->setDrawBuffers(drawBuffers);

    return currentFBO;
}

globjects::Framebuffer * RenderInterface::obtainFBO(size_t index, AbstractRenderTarget * renderTarget) const
{
    return obtainFBO(index, renderTarget, m_fbo.get(), m_defaultFBO.get());
}

globjects::Framebuffer * RenderInterface::obtainFBO(size_t index, AbstractRenderTarget * renderTarget, globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO)
{
    auto attachmentIndex = gl::GL_COLOR_ATTACHMENT0 + index;

    if (renderTarget->underlyingAttachmentType() == AttachmentType::Depth)
    {
        attachmentIndex = gl::GL_DEPTH_ATTACHMENT;
    }

    if (renderTarget->underlyingAttachmentType() == AttachmentType::Stencil)
    {
        attachmentIndex = gl::GL_STENCIL_ATTACHMENT;
    }

    if (renderTarget->underlyingAttachmentType() == AttachmentType::DepthStencil)
    {
        attachmentIndex = gl::GL_DEPTH_STENCIL_ATTACHMENT;
    }

    switch (renderTarget->currentTargetType())
    {
    case RenderTargetType::DefaultFBOAttachment:
        return defaultFBO;

    case RenderTargetType::UserDefinedFBOAttachment:
        {
            const auto fboAttachment = fbo->getAttachment(attachmentIndex);
            const auto targetAttachment = renderTarget->framebufferAttachment();

            const auto targetAttachedTexture = static_cast<globjects::AttachedTexture *>(targetAttachment);
            const auto targetAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(targetAttachment);
            const auto fboAttachedTexture = static_cast<globjects::AttachedTexture *>(fboAttachment);
            const auto fboAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(fboAttachment);

            if (!fboAttachment)
            {
                if (targetAttachment->isTextureAttachment())
                {
                    fbo->attachTexture(attachmentIndex, targetAttachedTexture->texture());
                }
                else
                {
                    fbo->attachRenderBuffer(attachmentIndex, targetAttachedRenderbuffer->renderBuffer());
                }
            }
            else if (fboAttachment->isTextureAttachment() && (!targetAttachment->isTextureAttachment() || fboAttachedTexture->texture() != targetAttachedTexture->texture()))
            {
                fbo->attachTexture(attachmentIndex, targetAttachedTexture->texture());
            }
            else if (fboAttachment->isRenderBufferAttachment() && (!targetAttachment->isRenderBufferAttachment() || fboAttachedRenderbuffer->renderBuffer() != targetAttachedRenderbuffer->renderBuffer()))
            {
                fbo->attachRenderBuffer(attachmentIndex, targetAttachedRenderbuffer->renderBuffer());
            }

            return fbo;
        }
        break;

    case RenderTargetType::Texture:
        {
            const auto attachment = fbo->getAttachment(attachmentIndex);

            const auto attachedTexture = static_cast<globjects::AttachedTexture *>(attachment);
            if (!attachment || !attachment->isTextureAttachment() || attachedTexture->texture() != renderTarget->textureAttachment())
            {
                fbo->attachTexture(attachmentIndex, renderTarget->textureAttachment());
            }

            return fbo;
        }
        break;

    case RenderTargetType::Renderbuffer:
        {
            const auto attachment = fbo->getAttachment(attachmentIndex);

            const auto attachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(attachment);
            if (!attachment->isRenderBufferAttachment() || attachedRenderbuffer->renderBuffer() != renderTarget->renderbufferAttachment())
            {
                fbo->attachRenderBuffer(attachmentIndex, renderTarget->renderbufferAttachment());
            }

            return fbo;
        }
        break;

    default:
        return nullptr;
    }
}

void RenderInterface::onContextInit()
{
    m_defaultFBO = globjects::Framebuffer::defaultFBO();
    m_fbo = cppassist::make_unique<globjects::Framebuffer>();
}

void RenderInterface::onContextDeinit()
{
    m_defaultFBO = nullptr;
    m_fbo = nullptr;
}


} // namespace gloperate
