/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractCapability.h>

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

    virtual const glm::vec3 & eye() const = 0;
    virtual void setEye(const glm::vec3 & eye) = 0;
    virtual const glm::vec3 & center() const = 0;
    virtual void setCenter(const glm::vec3 & center) = 0;
    virtual const glm::vec3 & up() const = 0;
    virtual void setUp(const glm::vec3 & up) = 0;

    virtual const glm::mat4 & view() const = 0;
    virtual const glm::mat4 & viewInverted() const = 0;
    virtual const glm::mat3 & normal() const = 0;
};


} // namespace gloperate
