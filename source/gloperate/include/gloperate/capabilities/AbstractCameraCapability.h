/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>

#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {

class AbstractViewportCapability;
class Camera;

/**
*  @brief
*    Base class for painter camera capabilities
*
*/
class GLOPERATE_API AbstractCameraCapability : public AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractCameraCapability(AbstractViewportCapability * viewportCapability);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCameraCapability();

    virtual void onViewportChanged() = 0;

    virtual void setCamera(Camera * camera) = 0;
    virtual Camera * getCamera() = 0;

protected:
    AbstractViewportCapability * m_viewportCapability;


};


} // namespace gloperate
