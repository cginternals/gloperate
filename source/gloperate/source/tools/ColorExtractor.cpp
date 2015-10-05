
#include <gloperate/tools/ColorExtractor.h>

#include <gloperate/base/RenderTargetType.h>


namespace gloperate
{


ColorExtractor::ColorExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   GBufferExtractor(viewportCapability, typedRenderTargetCapability, RenderTargetType::Color, glm::vec4(0.0))
{
}

ColorExtractor::~ColorExtractor()
{
}

glm::vec4 ColorExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    return GBufferExtractor::get(windowCoordinates);
}


} // namespace gloperate
