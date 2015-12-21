#pragma once
#include <string>
#include <glm/detail/type_vec3.hpp>
#include <gloperate/gloperate_api.h>

namespace gloperate {

enum class LightSourceType
{
    Undefined = 0,
    //! A directional light source has a well-defined direction
    //! but is infinitely far away. That's quite a good 
    //! approximation for sun light.
    Directional   = 1,

    //! A point light source has a well-defined position
    //! in space but no direction - it emits light in all
    //! directions. A normal bulb is a point light.
    Point         = 2,

    //! A spot light source emits light in a specific 
    //! angle. It has a position and a direction it is pointing to.
    //! A good example for a spot light is a light spot in
    //! sport arenas.
    Spot          = 3,

    //! The generic light level of the world, including the bounces
    //! of all other lightsources.
    //! Typically, there's at most one ambient light in a scene.
    //! This light type doesn't have a valid position, direction, or
    //! other properties, just a color. 
    Ambient       = 4,
};

class GLOPERATE_API Light
{
public:
    Light();
    Light(const Light & other);

    /** The name of the light source.
    */
    std::string name() const;

    /** The type of the light source.
    *
    */
    LightSourceType type() const;
    
    /** Position of the light source in space.
    */
    glm::vec3 position() const;
    
    /** Direction of the light source in space. Relative to the
    *  transformation of the node corresponding to the light.
    *
    *  The direction is undefined for point lights. The vector
    *  may be normalized, but it needn't.
    */
    glm::vec3 direction() const;

    /** Diffuse color of the light source
    *
    *  The diffuse light color is multiplied with the diffuse
    *  material color to obtain the final color that contributes
    *  to the diffuse shading term.
    */
    glm::vec3 colorDiffuse() const;

    /** Specular color of the light source
    *
    *  The specular light color is multiplied with the specular
    *  material color to obtain the final color that contributes
    *  to the specular shading term.
    */
    glm::vec3 colorSpecular() const;

    /** Constant light attenuation factor.
    *
    *  The intensity of the light source at a given distance 'd' from
    *  the light's position is
    *  @code
    *  Atten = 1/( att0 + att1 * d + att2 * d*d)
    *  @endcode
    *  This member corresponds to the att0 variable in the equation.
    *  Naturally undefined for directional lights.
    */
    float attenuationConstant() const;

    /** Linear light attenuation factor.
    *
    *  The intensity of the light source at a given distance 'd' from
    *  the light's position is
    *  @code
    *  Atten = 1/( att0 + att1 * d + att2 * d*d)
    *  @endcode
    *  This member corresponds to the att1 variable in the equation.
    *  Naturally undefined for directional lights.
    */
    float attenuationLinear() const;

    /** Quadratic light attenuation factor.
    *
    *  The intensity of the light source at a given distance 'd' from
    *  the light's position is
    *  @code
    *  Atten = 1/( att0 + att1 * d + att2 * d*d)
    *  @endcode
    *  This member corresponds to the att2 variable in the equation.
    *  Naturally undefined for directional lights.
    */
    float attenuationQuadratic() const;

    /** Inner angle of a spot light's light cone.
    *
    *  The spot light has maximum influence on objects inside this
    *  angle. The angle is given in cos(angle).
    */
    float cosineCutoff() const;

    void setType(LightSourceType type);
    void setName(std::string name);
    void setPosition(glm::vec3 position);
    void setDirection(glm::vec3 direction);
    void setColorDiffuse(glm::vec3 colorDiffuse);
    void setColorSpecular(glm::vec3 colorSpecular);
    void setAttenuationLinear(float attenuatioLinear);
    void setAttenuationConstant(float attenuationConstant);
    void setAttenuationQuadratic(float attenuationQuadratic);
    void setCosineCutoff(float cosineCutoff);

protected:
    std::string m_name;
    LightSourceType m_type;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_colorDiffuse;
    glm::vec3 m_colorSpecular;
    float m_attenuationConstant;
    float m_attenuationLinear;
    float m_attenuationQuadratic;    
    float m_cosineCutoff;
};
}
