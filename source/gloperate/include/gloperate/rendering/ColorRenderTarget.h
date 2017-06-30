
#pragma once


#include <gloperate/rendering/AbstractRenderTarget.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Color render target that represents one color target we can render into
*
*    A render target can internally be: a texture, a renderbuffer,
*    a symbolic attachment of the default renderbuffer, or a user-defined
*    renderbuffer with attachment specification.
*/
class GLOPERATE_API ColorRenderTarget : public AbstractRenderTarget
{
public:
    using AbstractRenderTarget::AbstractRenderTarget;

    // Virtual AbstractRenderTarget interface
    virtual gl::GLint clearBufferDrawBuffer(size_t index) const override;
    virtual gl::GLenum drawBufferAttachment(size_t index) const override;
    virtual AttachmentType underlyingAttachmentType() const override;
    virtual gl::GLenum attachmentGLType() const override;
};


} // namespace gloperate
