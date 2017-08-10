
#include <gloperate/rendering/DepthRenderTarget.h>

#include <glbinding/gl/enum.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType DepthRenderTarget::attachmentType() const
{
    return AttachmentType::Depth;
}

gl::GLenum DepthRenderTarget::attachmentGLType() const
{
    return gl::GL_DEPTH;
}

gl::GLint DepthRenderTarget::clearBufferDrawBuffer(size_t /*index*/) const
{
    return 0;
}

gl::GLenum DepthRenderTarget::drawBufferAttachment(size_t /*index*/) const
{
    return gl::GL_NONE;
}


} // namespace gloperate
