
#pragma once


#include <glm/mat4x4.hpp>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class GLOPERATE_API CameraUtils
{
public:
    /**
    *  @brief
    *    Compute orthographic projection matrix from perspective parameters
    *
    *  @param[in] fovy
    *    same as for glm::perspective
    *  @param[in] aspectRatio
    *    same as for glm::perspective
    *  @param[in] zNear
    *    same as for glm::perspective
    *  @param[in] zFar
    *    same as for glm::perspective
    *  @param[in] syncDist
    *    distance of the plane that should keep constant size in both projections
    *    range: [zNear, zFar]
    */
    static glm::mat4 orthographicFromPerspective(double fovy, double aspectRatio, double zNear, double zFar, double syncDist);

    /**
    *  @brief
    *    Compute perspective projection matrix from orthographic parameters
    *
    *  @param[in] left
    *    same as for glm::ortho
    *    left = -right
    *  @param[in] right
    *    same as for glm::ortho
    *    left = -right
    *  @param[in] bottom
    *    same as for glm::ortho
    *    bottom = -top
    *  @param[in] top
    *    same as for glm::ortho
    *    bottom = -top
    *  @param[in] zNear
    *    same as for glm::ortho
    *  @param[in] zFar
    *    same as for glm::ortho
    *  @param[in] syncDist
    *    distance of the plane that should keep constant size in both projections
    *    range: [zNear, zFar]
    */
    static glm::mat4 perspectiveFromOrthographic(double left, double right, double bottom, double top, double zNear, double zFar, double syncDist);
};


} // namespace gloperate
