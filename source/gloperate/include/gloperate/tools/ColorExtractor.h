
#pragma once


#include <gloperate/tools/GBufferExtractor.h>


namespace gloperate
{


/**
*  @brief
*    Extract color values from G-Buffer
*/
class GLOPERATE_API ColorExtractor : protected GBufferExtractor
{
public:
    ColorExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~ColorExtractor();

    glm::vec4 get(const glm::ivec2 & windowCoordinates) const;
};


} // namespace gloperate
