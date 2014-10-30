/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/CameraCapability.h>

#include <glm/glm.hpp>

#include <gloperate/Camera.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>

namespace gloperate {


/**
*  @brief
*    Constructor
*/
CameraCapability::CameraCapability(AbstractViewportCapability * viewportCapability, Camera * camera)
: AbstractCameraCapability(viewportCapability)
, m_camera(camera)
{
}

/**
*  @brief
*    Destructor
*/
CameraCapability::~CameraCapability()
{
}

void CameraCapability::onViewportChanged()
{
    m_camera->setAspectRatio(glm::ivec2(m_viewportCapability->x(), m_viewportCapability->y()));
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
