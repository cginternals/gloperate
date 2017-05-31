
#pragma once

#include <gloperate/rendering/LightType.h>

#include <glm/vec3.hpp>


namespace gloperate
{


struct Light
{
    LightType type;
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 attenuationCoefficients;
};


} // namespace gloperate
