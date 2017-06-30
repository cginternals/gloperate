
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
: m_currentTargetType(RenderTargetType::Invalid)
, m_defaultFBOAttachment(gl::GL_NONE)
, m_texture(nullptr)
, m_renderbuffer(nullptr)
, m_userDefinedFBOAttachment(nullptr)
{
}

AbstractRenderTarget::~AbstractRenderTarget()
{
}

void AbstractRenderTarget::releaseTarget()
{
    switch (m_currentTargetType)
    {
    case RenderTargetType::Texture:
        m_texture = nullptr;
        break;
    case RenderTargetType::Renderbuffer:
        m_renderbuffer = nullptr;
        break;
    case RenderTargetType::DefaultFBOAttachment:
        m_defaultFBOAttachment = gl::GL_NONE;
        break;
    case RenderTargetType::UserDefinedFBOAttachment:
        m_userDefinedFBOAttachment = nullptr;
        break;
    case RenderTargetType::Invalid:
    default:
        m_defaultFBOAttachment = gl::GL_NONE;
        break;
    }

    m_currentTargetType = RenderTargetType::Invalid;
}

void AbstractRenderTarget::setTarget(globjects::Texture * texture)
{
    releaseTarget();

    m_currentTargetType = RenderTargetType::Texture;

    m_texture = texture;
}

void AbstractRenderTarget::setTarget(globjects::Renderbuffer * renderbuffer)
{
    releaseTarget();

    m_currentTargetType = RenderTargetType::Renderbuffer;

    m_renderbuffer = renderbuffer;
}

void AbstractRenderTarget::setTarget(gl::GLenum attachment)
{
    releaseTarget();

    m_currentTargetType = RenderTargetType::DefaultFBOAttachment;

    m_defaultFBOAttachment = attachment;
}

void AbstractRenderTarget::setTarget(globjects::FramebufferAttachment * fboAttachment)
{
    releaseTarget();

    m_currentTargetType = RenderTargetType::UserDefinedFBOAttachment;

    m_userDefinedFBOAttachment = fboAttachment;
}

RenderTargetType AbstractRenderTarget::currentTargetType() const
{
    return m_currentTargetType;
}

gl::GLenum AbstractRenderTarget::defaultFramebufferAttachment() const
{
    return m_defaultFBOAttachment;
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
    return m_userDefinedFBOAttachment;
}

bool AbstractRenderTarget::attachmentRequiresUserDefinedFramebuffer() const
{
    return m_currentTargetType == RenderTargetType::Texture
        || m_currentTargetType == RenderTargetType::Renderbuffer
        || m_currentTargetType == RenderTargetType::UserDefinedFBOAttachment;
}

gl::GLenum AbstractRenderTarget::clearBufferAttachment() const
{
    return attachmentRequiresUserDefinedFramebuffer()
        ? attachmentGLType()
        : m_defaultFBOAttachment;
}


} // namespace gloperate
