/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/CameraCapability.h>


namespace gloperate {


/**
*  @brief
*    Constructor
*/
CameraCapability::CameraCapability()
: AbstractCameraCapability()
{
}

/**
*  @brief
*    Destructor
*/
CameraCapability::~CameraCapability()
{
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
