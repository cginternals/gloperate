
#pragma once


#include <gloperate/rendering/AbstractRenderTarget.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Depth stencil render target that represents one depth stencil target
*    for depth and stencil tests during rasterization
*
*    A render target can internally be: a texture, a renderbuffer,
*    a symbolic attachment of the default renderbuffer, or a user-defined
*    renderbuffer with attachment specification.
*/
class GLOPERATE_API StencilRenderTarget : public AbstractRenderTarget
{
public:
    using AbstractRenderTarget::AbstractRenderTarget;

    // Virtual AbstractRenderTarget interface
    virtual AttachmentType underlyingAttachmentType() const override;
    virtual gl::GLenum attachmentGLType() const override;
};


} // namespace gloperate
