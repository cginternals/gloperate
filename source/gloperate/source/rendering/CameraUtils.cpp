
#include <gloperate/rendering/CameraUtils.h>

#include <glm/gtc/matrix_transform.hpp>


namespace gloperate
{


glm::mat4 CameraUtils::orthographicFromPerspective(double fovy, double aspectRatio, double zNear, double zFar, double syncDist)
{
    //compute ortho params from fovy and aspect assuming symmetry
    const auto right = zNear*glm::tan(fovy/2.0);
    const auto top = right / aspectRatio;
    const auto left = -right;
    const auto bottom = -top;
    
    const auto c = syncDist / zNear;
    
    return glm::ortho(c*left, c*right, c*bottom, c*top, zNear, zFar);
}

glm::mat4 CameraUtils::perspectiveFromOrthographic(double left, double right, double bottom, double top, double zNear, double zFar, double syncDist)
{
    const auto c = zNear / syncDist;

    const auto r = c*right;
    const auto t = c*top;

    const auto fovy = glm::atan(r / zNear) * 2; //assuming right = -left and bottom = -top
    const auto aspectRatio = (2 * r) / (2 * t);

    return glm::perspective(fovy, aspectRatio, zNear, zFar);
}


} // namespace gloperate
