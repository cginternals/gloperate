
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

    // Pick up new render target inputs and outputs
    stage->inputAdded .connect(this, &RenderInterface::registerRenderTargetInput);
    stage->outputAdded.connect(this, &RenderInterface::registerRenderTargetOutput);
}

RenderInterface::~RenderInterface()
{
}

void RenderInterface::updateRenderTargetOutputs()
{
    // Set output of each render target to signal an update to the pipeline
    pairwiseRenderTargetsDo([](Input<ColorRenderTarget *> * input, Output<ColorRenderTarget *> * output)
    {
        output->setValue(**input);
    });

    pairwiseRenderTargetsDo([](Input<DepthRenderTarget *> * input, Output<DepthRenderTarget *> * output)
    {
        output->setValue(**input);
    });

    pairwiseRenderTargetsDo([](Input<DepthStencilRenderTarget *> * input, Output<DepthStencilRenderTarget *> * output)
    {
        output->setValue(**input);
    });

    pairwiseRenderTargetsDo([](Input<StencilRenderTarget *> * input, Output<StencilRenderTarget *> * output)
    {
        output->setValue(**input);
    });
}

bool RenderInterface::renderTargetsAreCompatible() const
{
    // Quick exit if no render targets are registered
    if (m_colorRenderTargetInputs.empty() && m_depthRenderTargetInputs.empty() && m_depthStencilRenderTargetInputs.empty() && m_stencilRenderTargetInputs.empty())
    {
        return true;
    }

    // Get number of depth and stencil attachments
    auto numberOfDepthAttachments = m_depthRenderTargetOutputs.size() + m_depthStencilRenderTargetOutputs.size();
    auto numberOfStencilAttachments = m_depthStencilRenderTargetOutputs.size() + m_stencilRenderTargetOutputs.size();

    // Determine if all render targets refer to the default FBO
    auto allDefaultFramebufferAttachments =
        std::all_of(m_colorRenderTargetInputs.begin(), m_colorRenderTargetInputs.end(), [] (Input<ColorRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })

        && std::all_of(m_depthRenderTargetInputs.begin(), m_depthRenderTargetInputs.end(), [] (Input<DepthRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })

        && std::all_of(m_depthStencilRenderTargetInputs.begin(), m_depthStencilRenderTargetInputs.end(), [] (Input<DepthStencilRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })

        && std::all_of(m_stencilRenderTargetInputs.begin(), m_stencilRenderTargetInputs.end(), [] (Input<StencilRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return !renderTarget->attachmentRequiresUserDefinedFramebuffer();
        });

    // Determine if all render targets require a user-defined FBO
    auto allUserDefinedFramebufferAttachments =
        std::all_of(m_colorRenderTargetInputs.begin(), m_colorRenderTargetInputs.end(), [] (Input<ColorRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })

        && std::all_of(m_depthRenderTargetInputs.begin(), m_depthRenderTargetInputs.end(), [](Input<DepthRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })

        && std::all_of(m_depthStencilRenderTargetInputs.begin(), m_depthStencilRenderTargetInputs.end(), [](Input<DepthStencilRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        })

        && std::all_of(m_stencilRenderTargetInputs.begin(), m_stencilRenderTargetInputs.end(), [](Input<StencilRenderTarget *> * input)
        {
            if (input == nullptr) return true;

            auto renderTarget = **input;
            if (renderTarget == nullptr) return true;

            return renderTarget->attachmentRequiresUserDefinedFramebuffer();
        });

    // [TODO] Check this condition. If I add the default color-buffer and a user-defined depth buffer, wouldn't both bools be false, hence equal?
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

void RenderInterface::pairwiseRenderTargetsDo(std::function<void(Input<ColorRenderTarget *> *, Output<ColorRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_colorRenderTargetInputs.size(), m_colorRenderTargetOutputs.size())
        : std::min(m_colorRenderTargetInputs.size(), m_colorRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(
            m_colorRenderTargetInputs.size()  > i ? m_colorRenderTargetInputs.at(i)  : nullptr,
            m_colorRenderTargetOutputs.size() > i ? m_colorRenderTargetOutputs.at(i) : nullptr
        );
    }
}

void RenderInterface::pairwiseRenderTargetsDo(std::function<void(Input<DepthRenderTarget *> *, Output<DepthRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_depthRenderTargetInputs.size(), m_depthRenderTargetOutputs.size())
        : std::min(m_depthRenderTargetInputs.size(), m_depthRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(
            m_depthRenderTargetInputs.size()  > i ? m_depthRenderTargetInputs.at(i)  : nullptr,
            m_depthRenderTargetOutputs.size() > i ? m_depthRenderTargetOutputs.at(i) : nullptr
        );
    }
}

void RenderInterface::pairwiseRenderTargetsDo(std::function<void(Input<DepthStencilRenderTarget *> *, Output<DepthStencilRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_depthStencilRenderTargetInputs.size(), m_depthStencilRenderTargetOutputs.size())
        : std::min(m_depthStencilRenderTargetInputs.size(), m_depthStencilRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(
            m_depthStencilRenderTargetInputs.size()  > i ? m_depthStencilRenderTargetInputs.at(i)  : nullptr,
            m_depthStencilRenderTargetOutputs.size() > i ? m_depthStencilRenderTargetOutputs.at(i) : nullptr
        );
    }
}

void RenderInterface::pairwiseRenderTargetsDo(std::function<void(Input<StencilRenderTarget *> *, Output<StencilRenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::max(m_stencilRenderTargetInputs.size(), m_stencilRenderTargetOutputs.size())
        : std::min(m_stencilRenderTargetInputs.size(), m_stencilRenderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(
            m_stencilRenderTargetInputs.size()  > i ? m_stencilRenderTargetInputs.at(i)  : nullptr,
            m_stencilRenderTargetOutputs.size() > i ? m_stencilRenderTargetOutputs.at(i) : nullptr
        );
    }
}

globjects::Framebuffer * RenderInterface::obtainFBO() const
{
    assert(renderTargetsAreCompatible());

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

    if (renderTarget->attachmentType() == AttachmentType::Depth)
    {
        attachmentIndex = gl::GL_DEPTH_ATTACHMENT;
    }

    else if (renderTarget->attachmentType() == AttachmentType::Stencil)
    {
        attachmentIndex = gl::GL_STENCIL_ATTACHMENT;
    }

    else if (renderTarget->attachmentType() == AttachmentType::DepthStencil)
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

    default:
        return nullptr;
    }
}

void RenderInterface::onContextInit()
{
    m_defaultFBO = globjects::Framebuffer::defaultFBO();
    m_fbo        = cppassist::make_unique<globjects::Framebuffer>();
}

void RenderInterface::onContextDeinit()
{
    m_defaultFBO = nullptr;
    m_fbo        = nullptr;
}

void RenderInterface::registerRenderTargetInput(AbstractSlot * input)
{
    if (auto colorRenderTargetInput = dynamic_cast<Input<ColorRenderTarget *> *>(input))
    {
        m_colorRenderTargetInputs.push_back(colorRenderTargetInput);
        return;
    }

    if (auto depthRenderTargetInput = dynamic_cast<Input<DepthRenderTarget *> *>(input))
    {
        m_depthRenderTargetInputs.push_back(depthRenderTargetInput);
        return;
    }

    if (auto depthStencilRenderTargetInput = dynamic_cast<Input<DepthStencilRenderTarget *> *>(input))
    {
        m_depthStencilRenderTargetInputs.push_back(depthStencilRenderTargetInput);
        return;
    }

    if (auto stencilRenderTargetInput = dynamic_cast<Input<StencilRenderTarget *> *>(input))
    {
        m_stencilRenderTargetInputs.push_back(stencilRenderTargetInput);
        return;
    }
}

void RenderInterface::registerRenderTargetOutput(AbstractSlot * output)
{
    if (auto colorRenderTargetOutput = dynamic_cast<Output<ColorRenderTarget *> *>(output))
    {
        m_colorRenderTargetOutputs.push_back(colorRenderTargetOutput);
        return;
    }

    if (auto depthRenderTargetOutput = dynamic_cast<Output<DepthRenderTarget *> *>(output))
    {
        m_depthRenderTargetOutputs.push_back(depthRenderTargetOutput);
        return;
    }

    if (auto depthStencilRenderTargetOutput = dynamic_cast<Output<DepthStencilRenderTarget *> *>(output))
    {
        m_depthStencilRenderTargetOutputs.push_back(depthStencilRenderTargetOutput);
        return;
    }

    if (auto stencilRenderTargetOutput = dynamic_cast<Output<StencilRenderTarget *> *>(output))
    {
        m_stencilRenderTargetOutputs.push_back(stencilRenderTargetOutput);
        return;
    }
}


} // namespace gloperate
