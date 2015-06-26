
#pragma once


#include <gloperate/tools/GBufferExtractor.h>


namespace gloperate
{


class GLOPERATE_API ObjectIdExtractor : protected GBufferExtractor
{
public:
    ObjectIdExtractor(
        AbstractViewportCapability * viewportCapability,
        AbstractTypedRenderTargetCapability * typedRenderTargetCapability);

    virtual ~ObjectIdExtractor();

    int get(const glm::ivec2 & windowCoordinates) const;
};


} // namespace gloperate
