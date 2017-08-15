
#pragma once


#include <map>

#include <cppexpose/reflection/Object.h>

#include <gloperate/base/ChronoTimer.h>


namespace gloperate
{


class Environment;


/**
*  @brief
*    Manager for scripting timers
*/
class GLOPERATE_API TimerManager : public cppexpose::Object
{
public:
    struct Timer
    {
        bool                active;     ///< 'true' if timer is active, else 'false'
        bool                singleShot; ///< 'true' if timer fires only once, else 'false'
        float               interval;   ///< Interval (in seconds)
        float               remaining;  ///< Remaining time (in seconds)
        cppexpose::Function func;       ///< Script function which is called

        Timer()
        : active(false)
        , singleShot(false)
        , interval(0.0f)
        , remaining(0.0f)
        {
        }

        ~Timer()
        {
        }
    };


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the manager belongs (must NOT be null!)
    */
    TimerManager(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    ~TimerManager();

    // No copying
    TimerManager(const TimerManager &) = delete;
    TimerManager & operator=(const TimerManager &) = delete;

    /**
    *  @brief
    *    Update timing
    *
    *  @remarks
    *    This signature measures the time since the last call
    *    for the time delta. It should usually be used in
    *    interactive applications.
    */
    void update();

    /**
    *  @brief
    *    Update timing
    *
    *  @param[in] delta
    *    Time delta (in seconds)
    *
    *  @remarks
    *    This signature can be used to provide a specific time delta,
    *    e.g., when rendering videos at a fixed frame rate.
    */
    void update(float delta);


protected:
    // Scripting functions
    int  scr_start(int msec, const cppexpose::Variant & func);
    int  scr_once(int msec, const cppexpose::Variant & func);
    void scr_stop(int id);
    void scr_stopAll();
    int  scr_nextTick(const cppexpose::Variant & func);

    // Helper functions
    int  startTimer(const cppexpose::Variant & func, int msec, bool singleShot);
    void stopTimer(Timer * timer);
    void updateTimer(Timer * timer, float delta);


protected:
    Environment                         * m_environment; ///< Gloperate environment to which the manager belongs
    std::map<int, std::unique_ptr<Timer>> m_timers;      ///< List of activated timers
    int                                   m_nextId;      ///< Next timer ID
    gloperate::ChronoTimer                m_clock;       ///< Time measurement
};


} // namespace gloperate
