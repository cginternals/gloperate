
#include <gloperate/rendering/StencilRenderTarget.h>

#include <glbinding/gl/enum.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType StencilRenderTarget::underlyingAttachmentType() const
{
    return AttachmentType::Stencil;
}

gl::GLenum StencilRenderTarget::attachmentGLType() const
{
    return gl::GL_STENCIL;
}

gl::GLint StencilRenderTarget::clearBufferDrawBuffer(size_t /*index*/) const
{
    return 0;
}

gl::GLenum StencilRenderTarget::drawBufferAttachment(size_t /*index*/) const
{
    return gl::GL_NONE;
}


} // namespace gloperate
