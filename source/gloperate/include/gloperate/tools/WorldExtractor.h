#pragma once

#include <gloperate/ext-includes-begin.h>
#include <glm/vec3.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/tools/GBufferExtractor.h>

namespace gloperate
{

class GLOPERATE_API WorldExtractor : protected GBufferExtractor
{
public:
    WorldExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~WorldExtractor();

    glm::vec3 get(const glm::ivec2 & windowCoordinates) const;
};

} // namespace gloperate
