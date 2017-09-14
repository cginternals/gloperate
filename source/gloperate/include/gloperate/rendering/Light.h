
#pragma once

#include <gloperate/rendering/LightType.h>

#include <glm/vec3.hpp>


namespace gloperate
{


struct Light
{
public:
    LightType type;
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 attenuationCoefficients;


public:
    bool operator==(const Light & other) const
    {
        return this == &other;
    }
};


} // namespace gloperate
