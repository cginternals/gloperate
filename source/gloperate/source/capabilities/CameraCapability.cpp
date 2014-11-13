/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/CameraCapability.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <gloperate/Camera.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>

namespace gloperate {


/**
*  @brief
*    Constructor
*/
CameraCapability::CameraCapability(
    AbstractViewportCapability * viewportCapability
,   Camera * camera
,   const glm::vec3 & eye
, const glm::vec3 & center
, const glm::vec3 & up)
: AbstractCameraCapability(viewportCapability)
, m_camera(camera)

, m_dirty(false)
, m_autoUpdate(false)

, m_eye(eye)
, m_center(center)
, m_up(up)
{
}

/**
*  @brief
*    Destructor
*/
CameraCapability::~CameraCapability()
{
}

bool CameraCapability::autoUpdating() const
{
    return m_autoUpdate;
}

void CameraCapability::setAutoUpdating(bool b)
{
    m_autoUpdate = b;

    if (m_dirty && m_autoUpdate)
    {
        update();
    }
}

void CameraCapability::invalidateMatrices() const
{
    m_view.invalidate();
    m_viewInverted.invalidate();
    //m_viewProjection.invalidate();
    //m_viewProjectionInverted.invalidate();
    m_normal.invalidate();
}

void CameraCapability::dirty(bool update)
{
    m_dirty = true;

    if (update || m_autoUpdate)
        this->update();
}

const glm::vec3 & CameraCapability::eye() const
{
    return m_eye;
}

void CameraCapability::setEye(const glm::vec3 & eye)
{
    if (eye == m_eye)
        return;

    m_eye = eye;
    dirty();
}

const glm::vec3 & CameraCapability::center() const
{
    return m_center;
}

void CameraCapability::setCenter(const glm::vec3 & center)
{
    if (center == m_center)
        return;

    m_center = center;
    dirty();
}

const glm::vec3 & CameraCapability::up() const
{
    return m_up;
}

void CameraCapability::setUp(const glm::vec3 & up)
{
    if (up == m_up)
        return;

    m_up = up;
    dirty();
}

const glm::mat4 & CameraCapability::view() const
{
    if (m_dirty)
        update();

    if (!m_view.isValid())
        m_view.setValue(glm::lookAt(m_eye, m_center, m_up));

    return m_view.value();
}

const glm::mat4 & CameraCapability::viewInverted() const
{
    if (m_dirty)
        update();

    if (!m_viewInverted.isValid())
        m_viewInverted.setValue(glm::inverse(view()));

    return m_viewInverted.value();
}

const glm::mat3 & CameraCapability::normal() const
{
    if (m_dirty)
        update();

    if (!m_normal.isValid())
        m_normal.setValue(glm::inverseTranspose(glm::mat3(view())));

    return m_normal.value();
}

void CameraCapability::update() const
{
    if (!m_dirty)
        return;

    invalidateMatrices();

    m_dirty = false;

    const_cast<CameraCapability*>(this)->changed();
}

void CameraCapability::changed()
{
    invalidated();
}

void CameraCapability::onViewportChanged()
{
    m_camera->setAspectRatio(glm::ivec2(m_viewportCapability->width(), m_viewportCapability->height()));
}

void CameraCapability::setCamera(Camera * camera)
{
    if (m_camera != camera)
    {
        m_camera = camera;
        setChanged(true);
    }
}

Camera * CameraCapability::getCamera()
{
    return m_camera;
}

} // namespace gloperate
