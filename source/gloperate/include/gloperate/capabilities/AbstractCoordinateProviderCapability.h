#pragma once

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>

namespace gloperate
{
class Camera;

/** \brief Provides a mapping from screen coordinates to world coordinates.

    This interface is inteded to be used by, e.g., navigation classes that require
    basic scene information based on the pointing devices (mouse, touch, ...) position.

    Note: there are multiple interfaces provided for objAt to allow reduction of 
    depthAt calls and reduction of viewProjection inversion.

    \see WorldInHandNavigation
*/
class GLOPERATE_API AbstractCoordinateProviderCapability : public AbstractCapability
{
public:
    static bool validDepth(const float depth);

public:
    AbstractCoordinateProviderCapability();

    virtual ~AbstractCoordinateProviderCapability();

    virtual float depthAt(const glm::ivec2 & windowCoordinates) const = 0;

    virtual glm::vec3 worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const = 0;
};

} // namespace gloperate
