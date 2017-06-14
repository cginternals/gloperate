
#include <gloperate/stages/base/ClearStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedRenderbuffer.h>
#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/RenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ClearStage, gloperate::Stage)


ClearStage::ClearStage(Environment * environment, const std::string & name)
: Stage(environment, "ClearStage", name)
, clear                    ("clear",                     this)
, viewport                 ("viewport",                  this)
, colorAttachment0         ("colorAttachment0",          this)
, clearColor0              ("clearColor0",               this)
, colorAttachment1         ("colorAttachment1",          this)
, clearColor1              ("clearColor1",               this)
, colorAttachment2         ("colorAttachment2",          this)
, clearColor2              ("clearColor2",               this)
, colorAttachment3         ("colorAttachment3",          this)
, clearColor3              ("clearColor3",               this)
, colorAttachment4         ("colorAttachment4",          this)
, clearColor4              ("clearColor4",               this)
, depthAttachment          ("depthAttachment",           this)
, depthValue               ("depthValue",                this)
, depthStencilAttachment   ("depthStencilAttachment",    this)
, stencilValue             ("stencilValue",              this)
, colorAttachment0Out      ("colorAttachment0Out",       this)
, colorAttachment1Out      ("colorAttachment1Out",       this)
, colorAttachment2Out      ("colorAttachment2Out",       this)
, colorAttachment3Out      ("colorAttachment3Out",       this)
, colorAttachment4Out      ("colorAttachment4Out",       this)
, depthAttachmentOut       ("depthAttachmentOut",        this)
, depthStencilAttachmentOut("depthStencilAttachmentOut", this)
{
}

ClearStage::~ClearStage()
{
}

void ClearStage::onContextInit(AbstractGLContext *)
{
    m_defaultFBO = globjects::Framebuffer::defaultFBO();
    m_fbo = cppassist::make_unique<globjects::Framebuffer>();
}

void ClearStage::onContextDeinit(AbstractGLContext *)
{
    m_defaultFBO = nullptr;
    m_fbo = nullptr;
}

void ClearStage::onProcess()
{
    if (!*clear)
    {
        // Setup OpenGL state
        gl::glScissor(viewport->x, viewport->y, viewport->z, viewport->w);
        gl::glEnable(gl::GL_SCISSOR_TEST);

        // Clear individual buffers
        clearColorAttachment       (0, *colorAttachment0,       *clearColor0              );
        clearColorAttachment       (1, *colorAttachment1,       *clearColor1              );
        clearColorAttachment       (2, *colorAttachment2,       *clearColor2              );
        clearColorAttachment       (3, *colorAttachment3,       *clearColor3              );
        clearColorAttachment       (4, *colorAttachment4,       *clearColor4              );
        clearDepthAttachment       (   *depthAttachment,        *depthValue               );
        clearDepthStencilAttachment(   *depthStencilAttachment, *depthValue, *stencilValue);

        // Reset OpenGL state
        gl::glDisable(gl::GL_SCISSOR_TEST);
    }

    // Update outputs
    colorAttachment0Out.setValue(&colorAttachment0);
    colorAttachment1Out.setValue(&colorAttachment1);
    colorAttachment2Out.setValue(&colorAttachment2);
    colorAttachment3Out.setValue(&colorAttachment3);
    colorAttachment4Out.setValue(&colorAttachment4);
    depthAttachmentOut.setValue(&depthAttachmentOut);
    depthStencilAttachmentOut.setValue(&depthStencilAttachmentOut);
}

void ClearStage::clearColorAttachment(unsigned char index, gloperate::RenderTarget * target, const glm::vec4 & clearColor)
{
    if (!target)
    {
        return;
    }

    const auto attachmentIndex = gl::GL_COLOR_ATTACHMENT0 + index;

    switch (target->type())
    {
    case RenderTargetType::DefaultFBOAttachment:
        m_defaultFBO->clearBuffer(target->defaultFramebufferAttachment(), 0, clearColor);
        break;

    case RenderTargetType::UserDefinedFBOAttachment:
        {
            const auto fboAttachment = m_fbo->getAttachment(attachmentIndex);
            const auto targetAttachment = target->framebufferAttachment();

            const auto targetAttachedTexture = static_cast<globjects::AttachedTexture *>(targetAttachment);
            const auto targetAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(targetAttachment);
            const auto fboAttachedTexture = static_cast<globjects::AttachedTexture *>(fboAttachment);
            const auto fboAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(fboAttachment);

            if (fboAttachment->isTextureAttachment() && (!targetAttachment->isTextureAttachment() || fboAttachedTexture->texture() != targetAttachedTexture->texture()))
            {
                m_fbo->attachTexture(attachmentIndex, targetAttachedTexture->texture());
            }
            else if (fboAttachment->isRenderBufferAttachment() && (!targetAttachment->isRenderBufferAttachment() || fboAttachedRenderbuffer->renderbuffer() != targetAttachedRenderbuffer->renderbuffer()))
            {
                m_fbo->attachTexture(attachmentIndex, targetAttachedRenderbuffer->renderbuffer);
            }

            m_fbo->clearBuffer(gl::GL_COLOR, index, clearColor);
        }
        break;

    case RenderTargetType::Texture:
        {
            const auto attachmentIndex = gl::GL_COLOR_ATTACHMENT0 + index;
            const auto attachment = m_fbo->getAttachment(attachmentIndex);

            const auto attachedTexture = static_cast<globjects::AttachedTexture *>(attachment);
            if (!attachment->isTextureAttachment() || attachedTexture->texture() != target->textureAttachment())
            {
                m_fbo->attachTexture(attachmentIndex, target->textureAttachment());
            }

            m_fbo->clearBuffer(gl::GL_COLOR, index, clearColor);
        }
        break;

    case RenderTargetType::Renderbuffer:
        {
            const auto attachmentIndex = gl::GL_COLOR_ATTACHMENT0 + index;
            const auto attachment = m_fbo->getAttachment(attachmentIndex);

            const auto attachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(attachment);
            if (!attachment->isRenderBufferAttachment() || attachedRenderbuffer->renderBuffer() != target->renderbufferAttachment())
            {
                m_fbo->attachRenderBuffer(attachmentIndex, target->renderbufferAttachment());
            }

            m_fbo->clearBuffer(gl::GL_COLOR, index, clearColor);
        }
        break;

    default:
        return false;
    }
}

void ClearStage::clearDepthAttachment(gloperate::RenderTarget * target, float clearDepth)
{
    if (!target)
    {
        return;
    }

    const auto attachmentIndex = gl::GL_DEPTH_ATTACHMENT;

    switch (target->type())
    {
    case RenderTargetType::DefaultFBOAttachment:
        m_defaultFBO->clearBuffer(target->defaultFramebufferAttachment(), clearDepth, 0);
        break;

    case RenderTargetType::UserDefinedFBOAttachment:
        {
            const auto fboAttachment = m_fbo->getAttachment(attachmentIndex);
            const auto targetAttachment = target->framebufferAttachment();

            const auto targetAttachedTexture = static_cast<globjects::AttachedTexture *>(targetAttachment);
            const auto targetAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(targetAttachment);
            const auto fboAttachedTexture = static_cast<globjects::AttachedTexture *>(fboAttachment);
            const auto fboAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(fboAttachment);

            if (fboAttachment->isTextureAttachment() && (!targetAttachment->isTextureAttachment() || fboAttachedTexture->texture() != targetAttachedTexture->texture()))
            {
                m_fbo->attachTexture(attachmentIndex, targetAttachedTexture->texture());
            }
            else if (fboAttachment->isRenderBufferAttachment() && (!targetAttachment->isRenderBufferAttachment() || fboAttachedRenderbuffer->renderbuffer() != targetAttachedRenderbuffer->renderbuffer()))
            {
                m_fbo->attachTexture(attachmentIndex, targetAttachedRenderbuffer->renderbuffer);
            }

            m_fbo->clearBuffer(gl::GL_DEPTH, clearDepth, 0);
        }
        break;

    case RenderTargetType::Texture:
        {
            const auto attachment = m_fbo->getAttachment(attachmentIndex);

            const auto attachedTexture = static_cast<globjects::AttachedTexture *>(attachment);
            if (!attachment->isTextureAttachment() || attachedTexture->texture() != target->textureAttachment())
            {
                m_fbo->attachTexture(attachmentIndex, target->textureAttachment());
            }

            m_fbo->clearBuffer(gl::GL_DEPTH, clearDepth, 0);
        }
        break;

    case RenderTargetType::Renderbuffer:
        {
            const auto attachment = m_fbo->getAttachment(attachmentIndex);

            const auto attachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(attachment);
            if (!attachment->isRenderBufferAttachment() || attachedRenderbuffer->renderBuffer() != target->renderbufferAttachment())
            {
                m_fbo->attachRenderBuffer(attachmentIndex, target->renderbufferAttachment());
            }

            m_fbo->clearBuffer(gl::GL_DEPTH, clearDepth, 0);
        }
        break;

    default:
        return false;
    }
}

void ClearStage::clearDepthStencilAttachment(gloperate::RenderTarget * target, float clearDepth, int clearStencil)
{
    if (!target)
    {
        return;
    }

    const auto attachmentIndex = gl::GL_DEPTH_STENCIL_ATTACHMENT;

    switch (target->type())
    {
    case RenderTargetType::DefaultFBOAttachment:
        m_defaultFBO->clearBuffer(target->defaultFramebufferAttachment(), 0, clearDepth, clearStencil);
        break;

    case RenderTargetType::UserDefinedFBOAttachment:
        {
            const auto fboAttachment = m_fbo->getAttachment(attachmentIndex);
            const auto targetAttachment = target->framebufferAttachment();

            const auto targetAttachedTexture = static_cast<globjects::AttachedTexture *>(targetAttachment);
            const auto targetAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(targetAttachment);
            const auto fboAttachedTexture = static_cast<globjects::AttachedTexture *>(fboAttachment);
            const auto fboAttachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(fboAttachment);

            if (fboAttachment->isTextureAttachment() && (!targetAttachment->isTextureAttachment() || fboAttachedTexture->texture() != targetAttachedTexture->texture()))
            {
                m_fbo->attachTexture(attachmentIndex, targetAttachedTexture->texture());
            }
            else if (fboAttachment->isRenderBufferAttachment() && (!targetAttachment->isRenderBufferAttachment() || fboAttachedRenderbuffer->renderbuffer() != targetAttachedRenderbuffer->renderbuffer()))
            {
                m_fbo->attachTexture(attachmentIndex, targetAttachedRenderbuffer->renderbuffer);
            }

            m_fbo->clearBuffer(gl::GL_DEPTH_STENCIL, clearDepth, clearStencil);
        }
        break;

    case RenderTargetType::Texture:
        {
            const auto attachment = m_fbo->getAttachment(attachmentIndex);

            const auto attachedTexture = static_cast<globjects::AttachedTexture *>(attachment);
            if (!attachment->isTextureAttachment() || attachedTexture->texture() != target->textureAttachment())
            {
                m_fbo->attachTexture(attachmentIndex, target->textureAttachment());
            }

            m_fbo->clearBuffer(gl::GL_DEPTH_STENCIL, clearDepth, clearStencil);
        }
        break;

    case RenderTargetType::Renderbuffer:
        {
            const auto attachment = m_fbo->getAttachment(attachmentIndex);

            const auto attachedRenderbuffer = static_cast<globjects::AttachedRenderbuffer *>(attachment);
            if (!attachment->isRenderBufferAttachment() || attachedRenderbuffer->renderBuffer() != target->renderbufferAttachment())
            {
                m_fbo->attachRenderBuffer(attachmentIndex, target->renderbufferAttachment());
            }

            m_fbo->clearBuffer(gl::GL_DEPTH_STENCIL, clearDepth, clearStencil);
        }
        break;

    default:
        return false;
    }
}


} // namespace gloperate
