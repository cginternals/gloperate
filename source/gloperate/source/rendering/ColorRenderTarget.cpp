
#include <gloperate/rendering/ColorRenderTarget.h>

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType ColorRenderTarget::attachmentType() const
{
    return AttachmentType::Color;
}

gl::GLenum ColorRenderTarget::attachmentGLType() const
{
    return gl::GL_COLOR;
}

gl::GLint ColorRenderTarget::clearBufferDrawBuffer(size_t index) const
{
    return attachmentRequiresUserDefinedFramebuffer() ? index : 0;
}

gl::GLenum ColorRenderTarget::drawBufferAttachment(size_t index) const
{
    return attachmentRequiresUserDefinedFramebuffer()
        ? gl::GL_COLOR_ATTACHMENT0 + index
        : m_defaultFBOAttachment;
}


} // namespace gloperate
