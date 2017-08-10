
#include <gloperate/rendering/DepthStencilRenderTarget.h>

#include <glbinding/gl/enum.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType DepthStencilRenderTarget::attachmentType() const
{
    return AttachmentType::DepthStencil;
}

gl::GLenum DepthStencilRenderTarget::attachmentGLType() const
{
    return gl::GL_DEPTH_STENCIL;
}

gl::GLint DepthStencilRenderTarget::clearBufferDrawBuffer(size_t /*index*/) const
{
    return 0;
}

gl::GLenum DepthStencilRenderTarget::drawBufferAttachment(size_t /*index*/) const
{
    return gl::GL_NONE;
}


} // namespace gloperate
