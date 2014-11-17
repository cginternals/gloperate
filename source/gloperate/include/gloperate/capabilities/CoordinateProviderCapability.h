#pragma once

#include <gloperate/capabilities/AbstractCoordinateProviderCapability.h>
#include <gloperate/capabilities/AbstractCameraCapability.h>
#include <gloperate/capabilities/AbstractProjectionCapability.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/capabilities/AbstractTypedRenderTargetCapability.h>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class GLOPERATE_API CoordinateProviderCapability : public AbstractCoordinateProviderCapability
{
public:
    CoordinateProviderCapability(
        AbstractCameraCapability * cameraCapability,
        AbstractProjectionCapability * projectionCapability,
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~CoordinateProviderCapability();

    virtual float depthAt(const glm::ivec2 & windowCoordinates) const override;

    virtual glm::vec3 worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const override;
    virtual glm::vec3 unproject(const glm::ivec2 & windowCoordinates, float depth) const override;

protected:
    void onRenderTargetsChanged();

protected:
    AbstractCameraCapability * m_cameraCapability;
    AbstractProjectionCapability * m_projectionCapability;
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
    RenderTarget m_depthBuffer;
    RenderTarget m_geometryBuffer;
};

} // namespace gloperate
