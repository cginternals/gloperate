
#include <gloperate/rendering/AbstractRenderTarget.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Texture.h>


namespace gloperate
{


AbstractRenderTarget::AbstractRenderTarget()
: m_type(RenderTargetType::Invalid)
, m_attachment(gl::GL_NONE)
, m_texture(nullptr)
, m_renderbuffer(nullptr)
, m_userDefined(nullptr)
{
}

AbstractRenderTarget::~AbstractRenderTarget()
{
}

void AbstractRenderTarget::releaseTarget()
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

AttachmentType AbstractRenderTarget::attachmentType() const
{
    return m_attachmentType;
}

gl::GLenum AbstractRenderTarget::attachmentGLType() const
{
    switch (m_attachmentType)
    {
    case AttachmentType::Depth:
        return gl::GL_DEPTH;
    case AttachmentType::Stencil:
        return gl::GL_DEPTH_STENCIL;
    case AttachmentType::Color:
    default:
        return gl::GL_COLOR;
    }
}

void AbstractRenderTarget::setAttachmentType(AttachmentType attachmentType)
{
    m_attachmentType = attachmentType;
}

void AbstractRenderTarget::setTarget(globjects::Texture * texture)
{
    releaseTarget();

    m_type = RenderTargetType::Texture;

    m_texture = texture;
}

void AbstractRenderTarget::setTarget(globjects::Renderbuffer * renderbuffer)
{
    releaseTarget();

    m_type = RenderTargetType::Renderbuffer;

    m_renderbuffer = renderbuffer;
}

void AbstractRenderTarget::setTarget(gl::GLenum attachment)
{
    releaseTarget();

    m_type = RenderTargetType::DefaultFBOAttachment;

    m_attachment = attachment;
}

void AbstractRenderTarget::setTarget(globjects::FramebufferAttachment * fboAttachment)
{
    releaseTarget();

    m_type = RenderTargetType::UserDefinedFBOAttachment;

    m_userDefined = fboAttachment;
}

RenderTargetType AbstractRenderTarget::type() const
{
    return m_type;
}

gl::GLenum AbstractRenderTarget::defaultFramebufferAttachment() const
{
    return m_attachment;
}

globjects::Texture * AbstractRenderTarget::textureAttachment() const
{
    return m_texture;
}

globjects::Renderbuffer * AbstractRenderTarget::renderbufferAttachment() const
{
    return m_renderbuffer;
}

globjects::FramebufferAttachment * AbstractRenderTarget::framebufferAttachment() const
{
    return m_userDefined;
}

bool AbstractRenderTarget::attachmentRequiresUserDefinedFramebuffer() const
{
    return m_type == RenderTargetType::Texture
        || m_type == RenderTargetType::Renderbuffer
        || m_type == RenderTargetType::UserDefinedFBOAttachment;
}

gl::GLenum AbstractRenderTarget::attachmentBuffer() const
{
    return attachmentRequiresUserDefinedFramebuffer()
        ? attachmentGLType()
        : m_attachment;
}

gl::GLint AbstractRenderTarget::attachmentDrawBuffer(size_t index) const
{
    return attachmentRequiresUserDefinedFramebuffer()
        ? (m_attachmentType == AttachmentType::Color ? index : 0)
        : 0;
}


} // namespace gloperate
