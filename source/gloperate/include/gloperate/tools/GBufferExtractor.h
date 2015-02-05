#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

namespace gloperate
{

class AbstractViewportCapability;

class GLOPERATE_API GBufferExtractor
{
public:
    GBufferExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability,
        RenderTargetType type,
        const glm::vec4 & defaultValue);

    virtual ~GBufferExtractor();

    glm::vec4 get(const glm::ivec2 & windowCoordinates) const;

protected:
    AbstractViewportCapability * m_viewportCapability;
    AbstractTypedRenderTargetCapability * m_typedRenderTargetCapability;
    RenderTargetType m_type;
    glm::vec4 m_defaultValue;
};

} // namespace gloperate
