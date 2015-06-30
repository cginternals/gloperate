
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/types.h>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>


namespace gloperate
{


/**
*  @brief
*    Calculator for world coordinates based on camera and viewport settings
*
*    The coordinate provider provides helper functions to calculate
*    coordinates in a scene based on the camera and viewport settings
*    and the current depth texture. It is used primarily by interaction
*    and navigation techniques.
*
*  @see AbstractInteraction
*/
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
