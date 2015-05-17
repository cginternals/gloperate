#pragma once
#include <string>
#include <glm/detail/type_vec3.hpp>

namespace gloperate {

enum LightSourceType
{
    UNDIFINED = 0x0,
	//! A directional light source has a well-defined direction
	//! but is infinitely far away. That's quite a good 
	//! approximation for sun light.
	DIRECTIONAL   = 0x1,

	//! A point light source has a well-defined position
	//! in space but no direction - it emits light in all
	//! directions. A normal bulb is a point light.
	POINT         = 0x2,

	//! A spot light source emits light in a specific 
	//! angle. It has a position and a direction it is pointing to.
	//! A good example for a spot light is a light spot in
	//! sport arenas.
	SPOT          = 0x3,

	//! The generic light level of the world, including the bounces
	//! of all other lightsources.
	//! Typically, there's at most one ambient light in a scene.
	//! This light type doesn't have a valid position, direction, or
	//! other properties, just a color. 
	AMBIENT       = 0x4,
};

class Light
{
protected:
	/** The name of the light source.
	 */
	std::string mName;

	/** The type of the light source.
 	 *
	 */
	LightSourceType mType;

	/** Position of the light source in space. Relative to the
	 */
	glm::vec3 mPosition;

	/** Direction of the light source in space. Relative to the
	 *  transformation of the node corresponding to the light.
	 *
	 *  The direction is undefined for point lights. The vector
	 *  may be normalized, but it needn't.
	 */
    glm::vec3 mDirection;

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
	float mAttenuationConstant;

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
	float mAttenuationLinear;

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
	float mAttenuationQuadratic;

	/** Diffuse color of the light source
	 *
	 *  The diffuse light color is multiplied with the diffuse 
	 *  material color to obtain the final color that contributes
	 *  to the diffuse shading term.
	 */
    glm::vec3 mColorDiffuse;

	/** Specular color of the light source
	 *
	 *  The specular light color is multiplied with the specular
	 *  material color to obtain the final color that contributes
	 *  to the specular shading term.
	 */
    glm::vec3 mColorSpecular;

	/** Ambient color of the light source
	 *
	 *  The ambient light color is multiplied with the ambient
	 *  material color to obtain the final color that contributes
	 *  to the ambient shading term. Most renderers will ignore
	 *  this value it, is just a remaining of the fixed-function pipeline
	 *  that is still supported by quite many file formats.
	 */
    glm::vec3 mColorAmbient;

	/** Inner angle of a spot light's light cone.
	 *
	 *  The spot light has maximum influence on objects inside this
	 *  angle. The angle is given in radians. It is 2PI for point 
	 *  lights and undefined for directional lights.
	 */
	float mAngleInnerCone;

	/** Outer angle of a spot light's light cone.
	 *
	 *  The spot light does not affect objects outside this angle.
	 *  The angle is given in radians. It is 2PI for point lights and 
	 *  undefined for directional lights. The outer angle must be
	 *  greater than or equal to the inner angle.
	 *  It is assumed that the application uses a smooth
	 *  interpolation between the inner and the outer cone of the
	 *  spot light. 
	 */
	float mAngleOuterCone;

    Light()
        :	mType                 (UNDIFINED)
		,	mAttenuationConstant  (0.f)
		,   mAttenuationLinear    (1.f)
		,   mAttenuationQuadratic (0.f)
        ,	mAngleInnerCone       (0.f)
        ,	mAngleOuterCone       (0.f)
	{
	}


};
}
