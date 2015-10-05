
#include <gloperate/tools/NormalExtractor.h>

#include <gloperate/base/RenderTargetType.h>


namespace gloperate
{


NormalExtractor::NormalExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   GBufferExtractor(viewportCapability, typedRenderTargetCapability, RenderTargetType::Normal, glm::vec4(0.0))
{
}

NormalExtractor::~NormalExtractor()
{
}

glm::vec3 NormalExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    return glm::vec3(GBufferExtractor::get(windowCoordinates)) * glm::vec3(2.0) - glm::vec3(1.0);
}


} // namespace gloperate
