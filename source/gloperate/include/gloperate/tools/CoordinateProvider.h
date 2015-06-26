#pragma once

#include <glbinding/gl/types.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

namespace gloperate
{

class GLOPERATE_API CoordinateProvider
{
public:
    CoordinateProvider(
        AbstractCameraCapability * cameraCapability,
        AbstractProjectionCapability * projectionCapability,
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~CoordinateProvider();

    virtual float depthAt(const glm::ivec2 & windowCoordinates) const;

    virtual glm::vec3 worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const;
    virtual glm::vec3 unproject(const glm::ivec2 & windowCoordinates, float depth) const;

protected:
    AbstractCameraCapability * m_cameraCapability;
    AbstractProjectionCapability * m_projectionCapability;
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
};

} // namespace gloperate
