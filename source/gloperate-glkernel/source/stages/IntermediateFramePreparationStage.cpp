
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
    m_defaultFBO = globjects::Framebuffer::defaultFBO();
    m_fbo = cppassist::make_unique<globjects::Framebuffer>();
    m_targetFBO = cppassist::make_unique<globjects::Framebuffer>();
}

void IntermediateFramePreparationStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_defaultFBO = nullptr;
    m_fbo = nullptr;
    m_targetFBO = nullptr;
}

void IntermediateFramePreparationStage::onProcess()
{
    if (!renderInterface.allRenderTargetsCompatible())
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

        auto sourceFBO = renderInterface.configureFBO(0, *intermediateRenderTarget, m_fbo.get(), m_defaultFBO.get());
        auto sourceAttachment = (*intermediateRenderTarget)->drawBufferAttachment(0);

        auto targetFBO = m_targetFBO.get();
        auto targetAttachment = gl::GL_COLOR_ATTACHMENT0;

        targetFBO->attachTexture(gl::GL_COLOR_ATTACHMENT0, *intermediateFrameTexture);

        sourceFBO->blit(sourceAttachment, rect, targetFBO, targetAttachment, rect, gl::GL_COLOR_BUFFER_BIT, gl::GL_NEAREST);
    }

    intermediateFrameTextureOut.setValue(*intermediateFrameTexture);
}


} // namespace gloperate_glkernel
