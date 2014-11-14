/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/capabilities/AbstractCapability.h>

#include <glm/glm.hpp>


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
    AbstractCameraCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCameraCapability();

    virtual const glm::mat4 & view() const = 0;
    virtual const glm::mat4 & viewInverted() const = 0;
};


} // namespace gloperate
