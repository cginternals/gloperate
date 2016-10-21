
#pragma once

#include <gloperate/rendering/LightType.h>

#include <glm/vec3.hpp>


namespace gloperate
{


struct LightDefinition
{
    LightType type;
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 attenuationCoefficients;
};


}
