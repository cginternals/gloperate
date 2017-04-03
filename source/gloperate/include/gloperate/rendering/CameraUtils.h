
#pragma once


#include <glm/mat4x4.hpp>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

/**
*  @brief
*    Compute orthographic projection matrix from perspective parameters
*
*  @param[in] fovy, aspectRatio, zNear, zFar
*    same as for glm::perspective
*
* @param[in] syncDist
*    distance of the plane that should keep constant size in both projections
*    range: [zNear, zFar]
*/
GLOPERATE_API glm::mat4 orthographicFromPerspective(float fovy, float aspectRatio, float zNear, float zFar, float syncDist);

/**
*  @brief
*    Compute perspective projection matrix from orthographic parameters
*
*  @param[in] left, right, bottom, top, zNear, zFar
*    same as for glm::ortho
*    left = -right; bottom = -top
*
* @param[in] syncDist
*    distance of the plane that should keep constant size in both projections
*    range: [zNear, zFar]
*/
GLOPERATE_API glm::mat4 perspectiveFromOrthographic(float left, float right, float bottom, float top, float zNear, float zFar, float syncDist);


} // namespace gloperate
