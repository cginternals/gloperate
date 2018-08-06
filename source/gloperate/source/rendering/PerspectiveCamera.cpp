
#include <gloperate/rendering/PerspectiveCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


using namespace glm;


namespace gloperate
{


PerspectiveCamera::PerspectiveCamera(const vec3 & eye, const vec3 & center, const vec3 & up)
: AbstractCamera(eye, center, up)
, m_fovy(radians(40.f))
, m_aspect(1.f)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

float PerspectiveCamera::fovy() const
{
    return m_fovy;
}

void PerspectiveCamera::setFovy(const float fovy)
{
    if (std::abs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
        return;

    m_fovy = fovy;
    assert(m_fovy > 0.0);

    dirty();
}

float PerspectiveCamera::aspectRatio() const
{
    return m_aspect;
}

void PerspectiveCamera::setAspectRatio(float ratio)
{
    m_aspect = ratio;

    dirty();
}

void PerspectiveCamera::setAspectRatio(const ivec2 & viewport)
{
    m_aspect = static_cast<float>(viewport.x) / max(static_cast<float>(viewport.y), 1.f);

    dirty();
}

glm::mat4 PerspectiveCamera::computeProjectionMatrix() const
{
    return perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}


} // namespace gloperate
