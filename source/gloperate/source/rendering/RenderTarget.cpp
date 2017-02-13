
#include <gloperate/rendering/RenderTarget.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Texture.h>


namespace gloperate
{


RenderTarget::RenderTarget()
: m_type(RenderTargetType::Invalid)
, m_attachment(gl::GL_NONE)
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::releaseTarget()
{
    switch (m_type)
    {
    case RenderTargetType::Texture:
        m_texture = nullptr;
        break;
    case RenderTargetType::Renderbuffer:
        m_renderbuffer = nullptr;
        break;
    case RenderTargetType::DefaultFBOAttachment:
        m_attachment = gl::GL_NONE;
        break;
    case RenderTargetType::UserDefinedFBOAttachment:
        m_userDefined = nullptr;
        break;
    case RenderTargetType::Invalid:
    default:
        m_attachment = gl::GL_NONE;
        break;
    }

    m_type = RenderTargetType::Invalid;
}

void RenderTarget::setTarget(globjects::Texture *texture)
{
    releaseTarget();

    m_type = RenderTargetType::Texture;

    m_texture = texture;
}

void RenderTarget::setTarget(globjects::Renderbuffer *renderbuffer)
{
    releaseTarget();

    m_type = RenderTargetType::Renderbuffer;

    m_renderbuffer = renderbuffer;
}

void RenderTarget::setTarget(gl::GLenum attachment)
{
    releaseTarget();

    m_type = RenderTargetType::DefaultFBOAttachment;

    m_attachment = attachment;
}

void RenderTarget::setTarget(globjects::FramebufferAttachment *fboAttachment)
{
    releaseTarget();

    m_type = RenderTargetType::UserDefinedFBOAttachment;

    m_userDefined = fboAttachment;
}

void RenderTarget::bind(gl::GLenum bindingPoint, globjects::Framebuffer * fbo)
{
    assert(fbo != nullptr);
    assert(!fbo->isDefault());

    switch (m_type)
    {
    case RenderTargetType::Texture:
        fbo->attachTexture(bindingPoint, m_texture);
        break;
    case RenderTargetType::Renderbuffer:
        fbo->attachRenderBuffer(bindingPoint, m_renderbuffer);
        break;
    case RenderTargetType::DefaultFBOAttachment:
        // ToDo
        break;
    case RenderTargetType::UserDefinedFBOAttachment:
        // ToDo
        break;
    case RenderTargetType::Invalid:
    default:
        // [TODO] show error/warning?
        break;
    }
}

// void RenderTarget::unbind()
// {
//     // ToDo
// }


} // namespace gloperate
