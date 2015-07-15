
#include <gloperate/navigation/CoordinateProvider.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <gloperate/painter/Camera.h>
#include <gloperate/tools/DepthExtractor.h>


namespace gloperate
{


CoordinateProvider::CoordinateProvider(
    AbstractCameraCapability * cameraCapability,
    AbstractProjectionCapability * projectionCapability,
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   m_cameraCapability(cameraCapability)
,   m_projectionCapability(projectionCapability)
,   m_viewportCapability(viewportCapability)
,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
}

CoordinateProvider::~CoordinateProvider()
{
}

float CoordinateProvider::depthAt(const glm::ivec2 & windowCoordinates) const
{
    return DepthExtractor(m_viewportCapability, m_typedRenderTargetCapability).get(windowCoordinates);
}

glm::vec3 CoordinateProvider::worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const
{
    const float depth = depthAt(windowCoordinates);

    return unproject(windowCoordinates, depth);
}

glm::vec3 CoordinateProvider::unproject(const glm::ivec2 & windowCoordinates, float depth) const
{
    const glm::mat4 viewProjectionInverted = m_cameraCapability->viewInverted() * m_projectionCapability->projectionInverted();

    const float x = static_cast<float>(windowCoordinates.x);
    const float y = static_cast<float>(windowCoordinates.y);

    // transform viewport to [-1;+1] (including z!)

    const float width = 2.0f / static_cast<float>(m_viewportCapability->width());
    const float height = 2.0f / static_cast<float>(m_viewportCapability->height());

    const glm::vec4 p = glm::vec4(x * width - 1.f, 1.f - y * height, depth * 2.f - 1.f, 1.f);

    // unproject this point back to object space
    const glm::vec4 u = viewProjectionInverted * p;
    return glm::vec3(u) / u.w;
}


} // namespace gloperate
