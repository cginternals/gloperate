
#pragma once


#include <gloperate/rendering/AbstractRenderTarget.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Depth render target that represents one depth target for depth tests during rasterization
*
*    A render target can internally be: a texture, a renderbuffer,
*    a symbolic attachment of the default renderbuffer, or a user-defined
*    renderbuffer with attachment specification.
*/
class GLOPERATE_API DepthRenderTarget : public AbstractRenderTarget
{
public:
    using AbstractRenderTarget::AbstractRenderTarget;

    // Virtual AbstractRenderTarget interface
    virtual gl::GLint clearBufferDrawBuffer(size_t index) const override;
    virtual gl::GLenum drawBufferAttachment(size_t index) const override;
    virtual AttachmentType attachmentType() const override;
    virtual gl::GLenum attachmentGLType() const override;
};


} // namespace gloperate
