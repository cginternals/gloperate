
#include <gloperate/rendering/ColorRenderTarget.h>

#include <gloperate/rendering/AttachmentType.h>


namespace gloperate
{


AttachmentType ColorRenderTarget::underlyingAttachmentType() const
{
    return AttachmentType::Color;
}


} // namespace gloperate
