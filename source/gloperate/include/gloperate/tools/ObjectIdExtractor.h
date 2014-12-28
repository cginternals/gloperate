#pragma once

#include <glm/vec2.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractViewportCapability;
class AbstractTypedRenderTargetCapability;

class GLOPERATE_API ObjectIdExtractor
{
public:
    ObjectIdExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~ObjectIdExtractor();

    int get(const glm::ivec2 & windowCoordinates) const;

protected:
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
};

} // namespace gloperate
