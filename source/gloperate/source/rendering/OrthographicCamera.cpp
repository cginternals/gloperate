
#include <gloperate/rendering/OrthographicCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


using namespace glm;


namespace gloperate
{


OrthographicCamera::OrthographicCamera(const vec3 & eye, const vec3 & center, const vec3 & up)
: AbstractCamera(eye, center, up)
, m_viewbox({ -1.0, 1.0, -1.0, 1.0 })
{
}

OrthographicCamera::~OrthographicCamera()
{
}

const glm::vec4 & OrthographicCamera::viewbox() const
{
    return m_viewbox;
}

void OrthographicCamera::setViewbox(const glm::vec4 & viewbox)
{
    m_viewbox = viewbox;

    dirty();
}

void OrthographicCamera::setViewbox(float left, float right, float bottom, float top)
{
    m_viewbox = { left, right, bottom, top };

    dirty();
}

glm::mat4 OrthographicCamera::computeProjectionMatrix() const
{
    return ortho(m_viewbox.x, m_viewbox.y, m_viewbox.z, m_viewbox.w, m_zNear, m_zFar);
}


} // namespace gloperate
