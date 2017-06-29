
#include <gloperate/rendering/DepthStencilRenderTarget.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType DepthStencilRenderTarget::underlyingAttachmentType() const
{
    return AttachmentType::DepthStencil;
}


} // namespace gloperate
