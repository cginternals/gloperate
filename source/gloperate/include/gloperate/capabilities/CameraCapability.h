/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/capabilities/AbstractCameraCapability.h>


namespace gloperate {

/**
*  @brief
*    Base class for painter camera capabilities
*
*/
class GLOPERATE_API CameraCapability : public AbstractCameraCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    CameraCapability(AbstractViewportCapability * viewportCapability, Camera * camera = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~CameraCapability();

    virtual void onViewportChanged() override;

    virtual void setCamera(Camera * camera) override;
    virtual Camera * getCamera() override;

protected:
    Camera * m_camera;

};


} // namespace gloperate
