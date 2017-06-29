
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


} // namespace gloperate
