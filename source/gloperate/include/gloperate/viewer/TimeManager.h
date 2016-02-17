
#pragma once


#include <list>

#include <gloperate/gloperate_api.h>


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
    *  @param[in] delta
    *    Time delta (in milliseconds)
    */
    void update(unsigned int delta);


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


protected:
    ViewerContext      * m_viewerContext; ///< Viewer context to which the manager belongs
    std::list<Timer *>   m_timers;        ///< List of registered timers
};


} // namespace gloperate
