#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractViewportCapability;
class AbstractTypedRenderTargetCapability;

class GLOPERATE_API ColorExtractor
{
public:
    ColorExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~ColorExtractor();

    glm::vec4 get(const glm::ivec2 & windowCoordinates) const;

protected:
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
};

} // namespace gloperate
