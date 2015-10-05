
#include <gloperate/tools/DepthExtractor.h>

#include <limits>

#include <gloperate/base/RenderTargetType.h>


namespace gloperate
{


DepthExtractor::DepthExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   GBufferExtractor(viewportCapability, typedRenderTargetCapability, RenderTargetType::Depth, glm::vec4(1.0))
{
}

DepthExtractor::~DepthExtractor()
{
}

float DepthExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    return GBufferExtractor::get(windowCoordinates).x;
}

bool DepthExtractor::isValidDepth(const float depth)
{
    return depth < (1.f - std::numeric_limits<float>::epsilon());
}


} // namespace gloperate
