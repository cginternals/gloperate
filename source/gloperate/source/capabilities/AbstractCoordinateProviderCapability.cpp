#include <gloperate/capabilities/AbstractCoordinateProviderCapability.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <glm/glm.hpp>

#include <gloperate/Camera.h>

using namespace glm;

namespace gloperate
{
AbstractCoordinateProviderCapability::AbstractCoordinateProviderCapability()
:AbstractCapability()
{
}

AbstractCoordinateProviderCapability::~AbstractCoordinateProviderCapability()
{
}


bool AbstractCoordinateProviderCapability::validDepth(const float depth)
{
    return depth < (1.f - std::numeric_limits<float>::epsilon());
}

} // namespace gloperate
