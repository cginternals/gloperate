#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractViewportCapability;
class AbstractTypedRenderTargetCapability;

class GLOPERATE_API WorldExtractor
{
public:
    WorldExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~WorldExtractor();

    glm::vec3 get(const glm::ivec2 & windowCoordinates) const;

protected:
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
};

} // namespace gloperate
