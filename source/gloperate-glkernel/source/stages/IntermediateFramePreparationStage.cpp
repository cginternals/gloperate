
#include <gloperate-glkernel/stages/IntermediateFramePreparationStage.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/AttachmentType.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(IntermediateFramePreparationStage, gloperate::Stage)


IntermediateFramePreparationStage::IntermediateFramePreparationStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface  (this)
, intermediateRenderTarget("intermediateRenderTarget", this)
, intermediateFrameTexture("intermediateFrameTexture", this)
, intermediateFrameTextureOut("intermediateFrameTextureOut", this)
{
}

IntermediateFramePreparationStage::~IntermediateFramePreparationStage()
{
}

void IntermediateFramePreparationStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_targetFBO = cppassist::make_unique<globjects::Framebuffer>();

    renderInterface.onContextInit();
}

void IntermediateFramePreparationStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_targetFBO = nullptr;

    renderInterface.onContextDeinit();
}

void IntermediateFramePreparationStage::onProcess()
{
    if (!renderInterface.renderTargetsAreCompatible())
    {
        cppassist::warning("gloperate") << "Framebuffer configuration not compatible";

        return;
    }

    if (intermediateRenderTarget->currentTargetType() != gloperate::RenderTargetType::Texture
        || intermediateRenderTarget->textureAttachment() != *intermediateFrameTexture)
    {
        std::array<gl::GLint, 4> rect = {{
            static_cast<gl::GLint>(renderInterface.viewport->x),
            static_cast<gl::GLint>(renderInterface.viewport->y),
            static_cast<gl::GLint>(renderInterface.viewport->z),
            static_cast<gl::GLint>(renderInterface.viewport->w)
        }};

        auto sourceFBO = renderInterface.obtainFBO(0, *intermediateRenderTarget);
        auto sourceAttachment = (*intermediateRenderTarget)->drawBufferAttachment(0);

        auto targetFBO = m_targetFBO.get();
        auto targetAttachment = gl::GL_COLOR_ATTACHMENT0;

        targetFBO->attachTexture(targetAttachment, *intermediateFrameTexture);

        sourceFBO->blit(sourceAttachment, rect, targetFBO, targetAttachment, rect, gl::GL_COLOR_BUFFER_BIT, gl::GL_NEAREST);
    }

    intermediateFrameTextureOut.setValue(*intermediateFrameTexture);
}


} // namespace gloperate_glkernel
