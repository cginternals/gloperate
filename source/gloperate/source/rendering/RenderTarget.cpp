
#include <gloperate/rendering/RenderTarget.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Texture.h>


namespace gloperate
{


RenderTarget::RenderTarget()
: m_type(RenderTargetType::Invalid)
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::setTarget(globjects::Texture *texture)
{
    m_type = RenderTargetType::Texture;

    m_texture = texture;
}

void RenderTarget::setTarget(globjects::Renderbuffer *renderbuffer)
{
    m_type = RenderTargetType::Renderbuffer;

    m_renderbuffer = renderbuffer;
}

void RenderTarget::setTarget(gl::GLenum attachment)
{
    m_type = RenderTargetType::DefaultFBOAttachment;

    m_attachment = attachment;
}

void RenderTarget::setTarget(globjects::FramebufferAttachment *fboAttachment)
{
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
