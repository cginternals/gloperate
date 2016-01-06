
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>


namespace gloperate
{


class AbstractViewportCapability;


/**
*  @brief
*    G-Buffer data extractor base class
*
*    A g-buffer extractor allows to extract specific data
*    such as color buffers from a g-buffer. It works on a
*    Painter that has to support the AbstractViewportCapability
*    and the AbstractTypedRenderTargetCapability.
*/
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
