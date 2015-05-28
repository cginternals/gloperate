#pragma once
#include <string>
#include <glm/detail/type_vec3.hpp>
#include <gloperate/gloperate_api.h>

namespace gloperate {

enum LightSourceType
{
    UNDIFINED = 0,
	//! A directional light source has a well-defined direction
	//! but is infinitely far away. That's quite a good 
	//! approximation for sun light.
    DIRECTIONAL   = 1,

	//! A point light source has a well-defined position
	//! in space but no direction - it emits light in all
	//! directions. A normal bulb is a point light.
    POINT         = 2,

	//! A spot light source emits light in a specific 
	//! angle. It has a position and a direction it is pointing to.
	//! A good example for a spot light is a light spot in
	//! sport arenas.
    SPOT          = 3,

	//! The generic light level of the world, including the bounces
	//! of all other lightsources.
	//! Typically, there's at most one ambient light in a scene.
	//! This light type doesn't have a valid position, direction, or
	//! other properties, just a color. 
    AMBIENT       = 4,
};

class GLOPERATE_API Light
{
protected:
	/** The name of the light source.
	 */
    std::string m_name;

	/** The type of the light source.
 	 *
	 */
    LightSourceType m_type;

	/** Position of the light source in space. Relative to the
	 */
    glm::vec3 m_position;

	/** Direction of the light source in space. Relative to the
	 *  transformation of the node corresponding to the light.
	 *
	 *  The direction is undefined for point lights. The vector
	 *  may be normalized, but it needn't.
	 */
    glm::vec3 m_direction;

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
    float m_attenuationConstant;

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
    float m_attenuationLinear;

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
    float m_attenuationQuadratic;

	/** Diffuse color of the light source
	 *
	 *  The diffuse light color is multiplied with the diffuse 
	 *  material color to obtain the final color that contributes
	 *  to the diffuse shading term.
	 */
    glm::vec3 m_colorDiffuse;

	/** Specular color of the light source
	 *
	 *  The specular light color is multiplied with the specular
	 *  material color to obtain the final color that contributes
	 *  to the specular shading term.
	 */
    glm::vec3 m_colorSpecular;


	/** Inner angle of a spot light's light cone.
	 *
	 *  The spot light has maximum influence on objects inside this
     *  angle. The angle is given in cos(angle).
	 */
    float m_cosinusCutOff;


public:
    Light();
    Light(const Light & toCopy);

    LightSourceType type() const;
    std::string name() const;
    glm::vec3 position() const;
    glm::vec3 direction() const;
    glm::vec3 colorDiffuse() const;
    glm::vec3 colorSpecular() const;
    float attenuationConst() const;
    float attenuationLinear() const;
    float attenuationQuad() const;
    float cosinusCutOff() const;

    void type(int type);
    void name(std::string name);
    void position(glm::vec3 pos);
    void direction(glm::vec3 dir);
    void colorDiffuse(glm::vec3 colorDiff);
    void colorSpecular(glm::vec3 colorSpec);
    void attenuationLinear(float attLin);
    void attenuationConst(float attConst);
    void attenuationQuad(float attQuad);
    void cosinusCutOff(float cosCutOff);

};
}
