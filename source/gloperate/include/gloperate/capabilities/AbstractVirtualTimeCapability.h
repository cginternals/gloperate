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


/**
*  @brief
*    Capability that allows a painter to receive continues timing updates
*
*    If a painter supports this capability, it will be supplied with a virtual
*    time that it can use to calculate updates, e.g., animations. Also, if
*    a viewer has a painter with an enabled VirtualTimeCapability, it will
*    switch to continuous rendering mode and trigger automatic rendering updates.
*/
class GLOPERATE_API AbstractVirtualTimeCapability : public AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractVirtualTimeCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractVirtualTimeCapability();

    /**
    *  @brief
    *    Get virtual time
    *
    *  @return
    *    Current time (in seconds)
    */
    virtual float time() const = 0;

    /**
    *  @brief
    *    Set duration of a whole cycle (after that, time is reset to 0)
    *
    *  @param[in] duration
    *    Duration after which time is reset to 0 (in seconds)
    */
    virtual void setLoopDuration(float duration) = 0;

    /**
    *  @brief
    *    Update virtual time
    *
    *  @param[in] delta
    *    Time delta (in seconds)
    */
    virtual void update(float delta) = 0;


};


} // namespace gloperate
