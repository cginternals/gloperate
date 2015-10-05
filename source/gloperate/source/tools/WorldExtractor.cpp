
#include <gloperate/tools/WorldExtractor.h>

#include <gloperate/base/RenderTargetType.h>


namespace gloperate
{


WorldExtractor::WorldExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   GBufferExtractor(viewportCapability, typedRenderTargetCapability, RenderTargetType::Geometry, glm::vec4(0.0))
{
}

WorldExtractor::~WorldExtractor()
{
}

glm::vec3 WorldExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    return glm::vec3(GBufferExtractor::get(windowCoordinates));
}


} // namespace gloperate
