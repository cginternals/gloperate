/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractVirtualTimeCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractVirtualTimeCapability
*/
class GLOPERATE_API VirtualTimeCapability : public AbstractVirtualTimeCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    VirtualTimeCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VirtualTimeCapability();

    /**
    *  @brief
    *    Get virtual time
    *
    *  @return
    *    Current time (in seconds)
    */
    virtual float time() const override;

    /**
    *  @brief
    *    Set duration of a whole cycle (after that, time is reset to 0)
    *
    *  @param[in] duration
    *    Duration after which time is reset to 0 (in seconds)
    */
    virtual void setLoopDuration(float duration) override;

    /**
    *  @brief
    *    Update virtual time
    *
    *  @param[in] delta
    *    Time delta (in seconds)
    */
    virtual void update(float delta) override;


protected:
    /**
    *  @brief
    *    Normalize time by wrapping it at the loop duration
    */
    void normalizeTime();


protected:
    float m_duration;	/**< Duration after which time is reset to 0 (in seconds) */ 
    float m_time;	  	/**< Current time */


};


} // namespace gloperate
