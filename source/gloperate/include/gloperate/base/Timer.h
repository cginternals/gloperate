
#pragma once


#include <cppexpose/signal/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Environment;


/**
*  @brief
*    Timer class for executing timed tasks
*/
class GLOPERATE_API Timer
{
public:
    cppexpose::Signal<> elapsed; ///< Called when the timer has elapsed


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the timer belongs (must NOT be null!)
    */
    Timer(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Timer();

    /**
    *  @brief
    *    Check if timer is currently active
    *
    *  @return
    *    'true' if timer is active, else 'false'
    */
    bool isActive() const;

    /**
    *  @brief
    *    Start timer
    *
    *  @param[in] interval
    *    Interval (in seconds, 0.0 for continuous update in each main loop iteration)
    *  @param[in] singleShot
    *    If 'true', the timer fires only once, otherwise it will be restarted continously
    */
    void start(float interval, bool singleShot = false);

    /**
    *  @brief
    *    Stop timer
    */
    void stop();

    /**
    *  @brief
    *    Get interval
    *
    *  @return
    *    Interval (in seconds)
    */
    float interval() const;

    /**
    *  @brief
    *    Get remaining time
    *
    *  @return
    *    Remaining time (in seconds)
    */
    float remainingTime() const;

    /**
    *  @brief
    *    Update timer
    *
    *  @param[in] delta
    *    Time delta (in seconds)
    */
    void update(float delta);


protected:
    /**
    *  @brief
    *    Called when the timer has elapsed
    *
    *  @remarks
    *    In the default implementation, the signal elapsed() is emitted.
    *    This behavior can be overriden by derived classes.
    */
    virtual void onElapsed();


protected:
    Environment * m_environment; ///< Gloperate environment to which the timer belongs
    bool          m_active;      ///< 'true' if timer is active, else 'false'
    bool          m_singleShot;  ///< 'true' if timer fires only once, else 'false'
    float         m_interval;    ///< Interval (in seconds)
    float         m_remaining;   ///< Remaining time (in seconds)
};


} // namespace gloperate
