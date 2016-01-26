
#pragma once


#include <gloperate/painter/AbstractCapability.h>


namespace gloperate 
{


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
    *    Check if virtual time is enabled
    *
    *  @return
    *    'true' if enabled, else 'false'
    */
    virtual bool enabled() const = 0;

    /**
    *  @brief
    *    Enable or disable virtual time
    *
    *  @param[in] enabled
    *    'true' if enabled, else 'false'
    *
    *  @remarks
    *    When a virtual time capability is present on a painter and enabled,
    *    the window will activate continuous rendering and updates. If not
    *    enabled, the viewer will switch back to on-demand rendering and disable
    *    continuous updates until the capability has been switched back on.
    */
    virtual void setEnabled(bool enabled) = 0;

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
    *    Get virtual time delta since the last update
    *
    *  @return
    *    Time delta (in seconds)
    */
    virtual float delta() const = 0;

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
