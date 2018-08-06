
#include <gloperate/rendering/AbstractCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


using namespace glm;


namespace gloperate
{


AbstractCamera::AbstractCamera(const vec3 & eye, const vec3 & center, const vec3 & up)
: m_dirty(false)
, m_autoUpdate(false)
, m_eye(eye)
, m_center(center)
, m_up(up)
, m_zNear(0.1f)
, m_zFar(64.0f)
{
}

AbstractCamera::~AbstractCamera()
{
}

bool AbstractCamera::autoUpdating() const
{
    return m_autoUpdate;
}

void AbstractCamera::setAutoUpdating(bool autoUpdate)
{
    m_autoUpdate = autoUpdate;

    if (m_dirty && m_autoUpdate)
    {
        update();
    }
}

void AbstractCamera::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;

    const_cast<AbstractCamera*>(this)->changed();
}

const vec3 & AbstractCamera::eye() const
{
    return m_eye;
}

void AbstractCamera::setEye(const vec3 & eye)
{
    if (eye == m_eye)
        return;

    m_eye = eye;
    dirty();
}

const vec3 & AbstractCamera::center() const
{
    return m_center;
}

void AbstractCamera::setCenter(const vec3 & center)
{
    if (center == m_center)
        return;

    m_center = center;
    dirty();
}

const vec3 & AbstractCamera::up() const
{
    return m_up;
}

void AbstractCamera::setUp(const vec3 & up)
{
    if (up == m_up)
        return;

    m_up = up;
    dirty();
}

float AbstractCamera::zNear() const
{
    return m_zNear;
}

void AbstractCamera::setZNear(const float zNear)
{
    if (std::abs(zNear - m_zNear) < std::numeric_limits<float>::epsilon())
        return;

    m_zNear = zNear;
    assert(m_zNear > 0.0);

    dirty();
}

float AbstractCamera::zFar() const
{
    return m_zFar;
}

void AbstractCamera::setZFar(const float zFar)
{
    if (std::abs(zFar - m_zFar) < std::numeric_limits<float>::epsilon())
        return;

    m_zFar = zFar;
    assert(m_zFar > m_zNear);

    dirty();
}

const mat4 & AbstractCamera::viewMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewMatrix.isValid())
        m_viewMatrix.setValue(lookAt(m_eye, m_center, m_up));

    return m_viewMatrix.value();
}

const mat4 & AbstractCamera::projectionMatrix() const
{
    if (m_dirty)
        update();

    if (!m_projectionMatrix.isValid())
        m_projectionMatrix.setValue(computeProjectionMatrix());

    return m_projectionMatrix.value();
}

const mat4 & AbstractCamera::viewProjectionMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewProjectionMatrix.isValid())
        m_viewProjectionMatrix.setValue(projectionMatrix() * viewMatrix());
    
    return m_viewProjectionMatrix.value();
}

const mat4 & AbstractCamera::viewInvertedMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewInvertedMatrix.isValid())
        m_viewInvertedMatrix.setValue(inverse(viewMatrix()));

    return m_viewInvertedMatrix.value();
}

const mat4 & AbstractCamera::projectionInvertedMatrix() const
{
    if (m_dirty)
        update();

    if (!m_projectionInvertedMatrix.isValid())
        m_projectionInvertedMatrix.setValue(inverse(projectionMatrix()));

    return m_projectionInvertedMatrix.value();
}

const mat4 & AbstractCamera::viewProjectionInvertedMatrix() const
{
    if (m_dirty)
        update();

    if (!m_viewProjectionInvertedMatrix.isValid())
        m_viewProjectionInvertedMatrix.setValue(inverse(viewProjectionMatrix()));

    return m_viewProjectionInvertedMatrix.value();
}

const mat3 & AbstractCamera::normalMatrix() const
{
    if (m_dirty)
        update();

    if (!m_normalMatrix.isValid())
        m_normalMatrix.setValue(inverseTranspose(mat3(viewMatrix())));

    return m_normalMatrix.value();
}

void AbstractCamera::changed()
{
    invalidated();
}

void AbstractCamera::dirty(bool update)
{
    m_dirty = true;

    if (update || m_autoUpdate)
        this->update();
}

void AbstractCamera::invalidateMatrices() const
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
