#pragma once

#include <glm/vec2.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractViewportCapability;
class AbstractTypedRenderTargetCapability;

class GLOPERATE_API DepthExtractor
{
public:
    DepthExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~DepthExtractor();

    float get(const glm::ivec2 & windowCoordinates) const;
    static bool isValidDepth(const float depth);

protected:
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
};

} // namespace gloperate
