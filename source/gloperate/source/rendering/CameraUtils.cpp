
#include <gloperate/rendering/CameraUtils.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace gloperate
{


CameraUtils::CameraUtils()
{
}

glm::mat4 CameraUtils::orthographicFromPerspective(float fovy, float aspectRatio, float zNear, float zFar, float syncDist)
{
    //compute ortho params from fovy and aspect assuming symmetry
    const auto right = zNear*glm::tan(fovy/2.f);
    const auto top = right / aspectRatio;
    const auto left = -right;
    const auto bottom = -top;
    
    const auto c = syncDist / zNear;
    
    return glm::ortho(c*left, c*right, c*bottom, c*top, zNear, zFar);
}

glm::mat4 CameraUtils::perspectiveFromOrthographic(float left, float right, float bottom, float top, float zNear, float zFar, float syncDist)
{
    const auto c = zNear / syncDist;

    const auto r = c*right;
    const auto t = c*top;

    const auto fovy = glm::atan(r / zNear) * 2.f; //assuming right = -left and bottom = -top
    const auto aspectRatio = (2.f * r) / (2.f * t);

    return glm::perspective(fovy, aspectRatio, zNear, zFar);
}


} // namespace gloperate
