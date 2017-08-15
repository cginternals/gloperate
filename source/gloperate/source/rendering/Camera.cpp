
#include <gloperate/rendering/Camera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


using namespace glm;


namespace gloperate
{


Camera::Camera(const vec3 & eye, const vec3 & center, const vec3 & up)
: m_dirty(false)
, m_autoUpdate(false)
, m_eye(eye)
, m_center(center)
, m_up(up)
, m_fovy(radians(40.f))
, m_aspect(1.f)
, m_zNear(0.1f)
, m_zFar(64.0f)
{
}

Camera::~Camera()
{
}

bool Camera::autoUpdating() const
{
    return m_autoUpdate;
}

void Camera::setAutoUpdating(bool autoUpdate)
{
    m_autoUpdate = autoUpdate;

    if (m_dirty && m_autoUpdate)
    {
        update();
    }
}

void Camera::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;

    const_cast<Camera*>(this)->changed();
}

const vec3 & Camera::eye() const
{
    return m_eye;
}

void Camera::setEye(const vec3 & eye)
{
    if (eye == m_eye)
        return;

    m_eye = eye;
    dirty();
}

const vec3 & Camera::center() const
{
    return m_center;
}

void Camera::setCenter(const vec3 & center)
{
    if (center == m_center)
        return;

    m_center = center;
    dirty();
}

const vec3 & Camera::up() const
{
    return m_up;
}

void Camera::setUp(const vec3 & up)
{
    if (up == m_up)
        return;

    m_up = up;
    dirty();
}

float Camera::zNear() const
{
    return m_zNear;
}

void Camera::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
        return;

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    dirty();
}

float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
        return;

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    dirty();
}

float Camera::fovy() const
{
    return m_fovy;
}

void Camera::setFovy(const float fovy)
{
    if (std::abs(fovy - m_fovy) < std::numeric_limits<float>::epsilon())
        return;

    m_fovy = fovy;
    assert(m_fovy > 0.0);

    dirty();
}

float Camera::aspectRatio() const
{
    return m_aspect;
}

void Camera::setAspectRatio(float ratio)
{
    m_aspect = ratio;

    dirty();
}

void Camera::setAspectRatio(const ivec2 & viewport)
{
    m_aspect = static_cast<float>(viewport.x) / max(static_cast<float>(viewport.y), 1.f);

    dirty();
}

const mat4 & Camera::viewMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewMatrix.isValid())
        m_viewMatrix.setValue(lookAt(m_eye, m_center, m_up));

    return m_viewMatrix.value();
}

const mat4 & Camera::projectionMatrix() const
{
    if (m_dirty)
        update();

    if (!m_projectionMatrix.isValid())
        m_projectionMatrix.setValue(perspective(m_fovy, m_aspect, m_zNear, m_zFar));

    return m_projectionMatrix.value();
}

const mat4 & Camera::viewProjectionMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewProjectionMatrix.isValid())
        m_viewProjectionMatrix.setValue(projectionMatrix() * viewMatrix());
    
    return m_viewProjectionMatrix.value();
}

const mat4 & Camera::viewInvertedMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewInvertedMatrix.isValid())
        m_viewInvertedMatrix.setValue(inverse(viewMatrix()));

    return m_viewInvertedMatrix.value();
}

const mat4 & Camera::projectionInvertedMatrix() const
{
    if (m_dirty)
        update();

    if (!m_projectionInvertedMatrix.isValid())
        m_projectionInvertedMatrix.setValue(inverse(projectionMatrix()));

    return m_projectionInvertedMatrix.value();
}

const mat4 & Camera::viewProjectionInvertedMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewProjectionInvertedMatrix.isValid())
        m_viewProjectionInvertedMatrix.setValue(inverse(viewProjectionMatrix()));

    return m_viewProjectionInvertedMatrix.value();
}

const mat3 & Camera::normalMatrix() const
{
    if (m_dirty)
        update();

    if (!m_normalMatrix.isValid())
        m_normalMatrix.setValue(inverseTranspose(mat3(viewMatrix())));

    return m_normalMatrix.value();
}

void Camera::changed()
{
    invalidated();
}

void Camera::dirty(bool update)
{
    m_dirty = true;

    if (update || m_autoUpdate)
        this->update();
}

void Camera::invalidateMatrices() const
{
    m_viewMatrix.invalidate();
    m_viewInvertedMatrix.invalidate();
    m_projectionMatrix.invalidate();
    m_projectionInvertedMatrix.invalidate();
    m_viewProjectionMatrix.invalidate();
    m_viewProjectionInvertedMatrix.invalidate();
    m_normalMatrix.invalidate();
}


} // namespace gloperate
