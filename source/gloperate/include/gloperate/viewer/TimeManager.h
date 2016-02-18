
#pragma once


#include <list>

#include <gloperate/base/ChronoTimer.h>


namespace gloperate
{


class ViewerContext;
class Timer;


/**
*  @brief
*    Manager for virtual time and timers
*/
class GLOPERATE_API TimeManager
{
    friend class Timer;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the manager belongs (must NOT be null!)
    */
    TimeManager(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    ~TimeManager();

    /**
    *  @brief
    *    Update timing
    *
    *  @return
    *    'true' if there are any timers active, 'false' if not
    */
    bool update();

    /**
    *  @brief
    *    Get time since last update
    *
    *  @return
    *    Time delta (in seconds)
    */
    float timeDelta() const;


protected:
    /**
    *  @brief
    *    Register timer
    *
    *  @param[in] timer
    *    Timer (must NOT be null)
    *
    *  @remarks
    *    Does NOT take ownership over the timer, you must manage
    *    the lifetime of the timer by yourself, e.g., instanciate
    *    it directly, not via dynamic memory allocation.
    */
    void registerTimer(Timer * timer);

    /**
    *  @brief
    *    Unregister timer
    *
    *  @param[in] timer
    *    Timer (must NOT be null)
    */
    void unregisterTimer(Timer * timer);

    /**
    *  @brief
    *    Inform manager about activation of a timer
    *
    *  @remarks
    *    Used to keep track of the number of active timers.
    *    If no timer was active before, the clock will be reset
    *    in order to avoid large time deltas after resuming updates.
    */
    void activateTimer();

    /**
    *  @brief
    *    Inform manager about de-activation of a timer
    *
    *  @remarks
    *    Used to keep track of the number of active timers.
    */
    void deactivateTimer();


protected:
    ViewerContext          * m_viewerContext; ///< Viewer context to which the manager belongs
    unsigned int             m_activeTimers;  ///< Number of active timers
    std::list<Timer *>       m_timers;        ///< List of registered timers
    gloperate::ChronoTimer   m_clock;         ///< Time measurement
    float                    m_timeDelta;     ///< Time delta since last update (in seconds)
};


} // namespace gloperate
