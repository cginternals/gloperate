
#include <gloperate/rendering/DepthRenderTarget.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType DepthRenderTarget::underlyingAttachmentType() const
{
    return AttachmentType::Depth;
}


} // namespace gloperate
