
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <cmath>

#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedRenderbuffer.h>
#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/RenderTargetType.h>
#include <gloperate/rendering/RenderTarget.h>


namespace gloperate
{


RenderInterface::RenderInterface(Stage * stage)
: viewport("viewport", stage)
{
    // Hide inputs in property editor
    viewport.setOption("hidden", true);

    stage->inputAdded.connect( [this] (AbstractSlot * connectedInput) {
        auto renderTargetInput = dynamic_cast<Input<RenderTarget *> *>(connectedInput);

        if (renderTargetInput)
        {
            addRenderTargetInput(renderTargetInput);
        }
    });

    stage->outputAdded.connect( [this] (AbstractSlot * connectedOutput) {
        auto renderTargetOutput = dynamic_cast<Output<RenderTarget *> *>(connectedOutput);

        if (renderTargetOutput)
        {
            addRenderTargetOutput(renderTargetOutput);
        }
    });
}

RenderInterface::~RenderInterface()
{
}

bool RenderInterface::allRenderTargetsCompatible() const
{
    if (renderTargetInputSize() == 0)
    {
        return true;
    }

    auto numberOfDepthAttachments = std::count_if(m_renderTargetInputs.begin(), m_renderTargetInputs.end(), [](Input<RenderTarget *> * input) {
            return input ? (**input)->attachmentType() == AttachmentType::Depth || (**input)->attachmentType() == AttachmentType::DepthStencil : false;
        });

    auto allDefaultFramebufferAttachments = std::all_of(m_renderTargetInputs.begin(), m_renderTargetInputs.end(), [](Input<RenderTarget *> * input) {
        return input ? (**input)->attachmentRequiresUserDefinedFramebuffer() : true;
    });

    auto allUserDefinedFramebufferAttachments = std::all_of(m_renderTargetInputs.begin(), m_renderTargetInputs.end(), [](Input<RenderTarget *> * input) {
        return input ? !(**input)->attachmentRequiresUserDefinedFramebuffer() : true;
    });

    return allDefaultFramebufferAttachments != allUserDefinedFramebufferAttachments && numberOfDepthAttachments <= 1;
}

size_t RenderInterface::renderTargetInputSize() const
{
    return m_renderTargetInputs.size();
}

const std::vector<Input<RenderTarget *> *> & RenderInterface::renderTargetInputs() const
{
    return m_renderTargetInputs;
}

Input<RenderTarget *> * RenderInterface::renderTargetInput(size_t index) const
{
    return m_renderTargetInputs.size() > index ? m_renderTargetInputs.at(index) : nullptr;
}

RenderTarget * RenderInterface::inputRenderTarget(size_t index) const
{
    const auto input = renderTargetInput(index);

    return input ? **input : nullptr;
}

size_t RenderInterface::renderTargetOutputSize() const
{
    return m_renderTargetOutputs.size();
}

const std::vector<Output<RenderTarget *> *> & RenderInterface::renderTargetOutputs() const
{
    return m_renderTargetOutputs;
}

Output<RenderTarget *> * RenderInterface::renderTargetOutput(size_t index) const
{
    return m_renderTargetOutputs.size() > index ? m_renderTargetOutputs.at(index) : nullptr;
}

RenderTarget * RenderInterface::outputRenderTarget(size_t index) const
{
    const auto output = renderTargetOutput(index);

    return output ? **output : nullptr;
}

void RenderInterface::addRenderTargetInput(Input<RenderTarget *> * input)
{
    m_renderTargetInputs.push_back(input);
}

void RenderInterface::addRenderTargetOutput(Output<RenderTarget *> * input)
{
    m_renderTargetOutputs.push_back(input);
}

void RenderInterface::pairwiseRenderTargetsDo(std::function<void(Input <RenderTarget *> *, Output <RenderTarget *> *)> callback, bool includeIncompletePairs)
{
    const auto end = includeIncompletePairs
        ? std::min(m_renderTargetInputs.size(), m_renderTargetOutputs.size())
        : std::max(m_renderTargetInputs.size(), m_renderTargetOutputs.size());

    for (auto i = size_t(0); i < end; ++i)
    {
        callback(renderTargetInput(i), renderTargetOutput(i));
    }
}

globjects::Framebuffer * RenderInterface::configureFBO(globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO) const
{
    assert(allRenderTargetsCompatible());

    globjects::Framebuffer * currentFBO = nullptr;
    auto colorAttachmentIndex = size_t(0);
    for (auto input : m_renderTargetInputs)
    {
        auto nextFBO = configureFBO(colorAttachmentIndex, **input, fbo, defaultFBO);

        if (!currentFBO)
        {
            currentFBO = nextFBO;
        }

        assert(nextFBO == currentFBO);

        if ((**input)->attachmentType() == AttachmentType::Color)
        {
            ++colorAttachmentIndex;
        }
    }

    return currentFBO;
}

globjects::Framebuffer * RenderInterface::configureFBO(size_t index, RenderTarget * renderTarget, globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO)
{
    const auto attachmentIndex = gl::GL_COLOR_ATTACHMENT0 + index;

    switch (renderTarget->type())
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

            if (fboAttachment->isTextureAttachment() && (!targetAttachment->isTextureAttachment() || fboAttachedTexture->texture() != targetAttachedTexture->texture()))
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
            if (!attachment->isTextureAttachment() || attachedTexture->texture() != renderTarget->textureAttachment())
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


} // namespace gloperate
