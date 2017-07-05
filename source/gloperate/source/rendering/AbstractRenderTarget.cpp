
#include <gloperate/rendering/AbstractRenderTarget.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedRenderbuffer.h>
#include <globjects/AttachedTexture.h>
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

bool AbstractRenderTarget::matchesAttachment(const AbstractRenderTarget * other) const
{
    if (this == other)
    {
        return true;
    }

    if (currentTargetType() == other->currentTargetType())
    {
        switch (currentTargetType())
        {
        case RenderTargetType::Texture:
            if (textureAttachment() == nullptr || other->textureAttachment() == nullptr)
            {
                return false;
            }

            return textureAttachment()->id() == other->textureAttachment()->id();
        case RenderTargetType::Renderbuffer:
            if (renderbufferAttachment() == nullptr || other->renderbufferAttachment() == nullptr)
            {
                return false;
            }

            return renderbufferAttachment()->id() == other->renderbufferAttachment()->id();
        case RenderTargetType::DefaultFBOAttachment:
            return defaultFramebufferAttachment() == other->defaultFramebufferAttachment();
        case RenderTargetType::UserDefinedFBOAttachment:
            if (framebufferAttachment() == nullptr || other->framebufferAttachment() == nullptr)
            {
                return false;
            }

            if (framebufferAttachment() == other->framebufferAttachment())
            {
                return true;
            }

            if (framebufferAttachment()->isTextureAttachment() && other->framebufferAttachment()->isTextureAttachment())
            {
                return framebufferAttachment()->asTextureAttachment()->texture()->id() ==
                    other->framebufferAttachment()->asTextureAttachment()->texture()->id();
            }
            else if (framebufferAttachment()->isRenderBufferAttachment() && other->framebufferAttachment()->isRenderBufferAttachment())
            {
                return framebufferAttachment()->asRenderBufferAttachment()->renderBuffer()->id() ==
                    other->framebufferAttachment()->asRenderBufferAttachment()->renderBuffer()->id();
            }
            else
            {
                return false;
            }
        case RenderTargetType::Invalid:
        default:
            return false;
        }
    }
    else if (currentTargetType() == RenderTargetType::DefaultFBOAttachment || other->currentTargetType() == RenderTargetType::DefaultFBOAttachment)
    {
        return false;
    }
    else if ((currentTargetType() == RenderTargetType::Texture || currentTargetType() == RenderTargetType::Renderbuffer) &&
        (other->currentTargetType() == RenderTargetType::Texture || other->currentTargetType() == RenderTargetType::Renderbuffer))
    {
        return false;
    }
    else if (currentTargetType() == RenderTargetType::UserDefinedFBOAttachment)
    {
        if (other->currentTargetType() == RenderTargetType::Texture)
        {
            return framebufferAttachment()->isTextureAttachment()
                && framebufferAttachment()->asTextureAttachment()->texture()->id() == other->textureAttachment()->id();
        }
        else if (other->currentTargetType() == RenderTargetType::Renderbuffer)
        {
            return framebufferAttachment()->isRenderBufferAttachment()
                && framebufferAttachment()->asRenderBufferAttachment()->renderBuffer()->id() == other->renderbufferAttachment()->id();
        }
        else
        {
            assert(false);

            return false;
        }
    }
    else if (other->currentTargetType() == RenderTargetType::UserDefinedFBOAttachment)
    {
        if (currentTargetType() == RenderTargetType::Texture)
        {
            return other->framebufferAttachment()->isTextureAttachment()
                && other->framebufferAttachment()->asTextureAttachment()->texture()->id() == textureAttachment()->id();
        }
        else if (currentTargetType() == RenderTargetType::Renderbuffer)
        {
            return other->framebufferAttachment()->isRenderBufferAttachment()
                && other->framebufferAttachment()->asRenderBufferAttachment()->renderBuffer()->id() == renderbufferAttachment()->id();
        }
        else
        {
            assert(false);

            return false;
        }
    }
    else
    {
        assert(false);

        return false;
    }
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
