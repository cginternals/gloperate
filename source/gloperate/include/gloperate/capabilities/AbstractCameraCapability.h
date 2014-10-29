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
    AbstractCameraCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCameraCapability();

    virtual void setCamera(Camera * camera) = 0;
    virtual Camera * getCamera() = 0;

};


} // namespace gloperate
