
#pragma once


#include <chrono>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    High performance timer based on std::chrono
*
*    This timer class is for time measurement, not for
*    automated execution of tasks. See Environment
*    for an interface to automatically trigger events
*    based on timers.
*/
class GLOPERATE_API ChronoTimer
{
public:
    using Duration = std::chrono::duration<long long, std::nano>;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] start
    *    Start time measurement right away?
    *  @param[in] autoUpdate
    *    'true' if auto-update is on, else 'false'
    */
    ChronoTimer(bool start = true, bool autoUpdate = true);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ChronoTimer();

    /**
    *  @brief
    *    Get automatic update status
    *
    *  @return
    *    'true' if auto-update is on, else 'false'
    */
    bool autoUpdating() const;

    /**
    *  @brief
    *    Set automatic update status
    *
    *  @param[in] autoUpdate
    *    'true' if auto-update is on, else 'false'
    *
    *  @remarks
    *    If on, each call to elapsed() automatically calls update() afterwards.
    */
    void setAutoUpdating(bool autoUpdate);

    /**
    *  @brief
    *    Update timer
    *
    *    Updates the current time stamp and calculates the elapsed time
    *    since the start of the timer. The elapsed time can be retrieved
    *    by calling elapsed() and will not change until the next call
    *    to update(). If auto-updating is on, elapsed() automatically
    *    calls update() afterwards.
    */
    void update();

    /**
    *  @brief
    *    Get elapsed time since start
    *
    *  @return
    *    Elapsed time (in nano seconds)
    */
    Duration elapsed() const;

    /**
    *  @brief
    *    Check if timer is paused
    *
    *  @return
    *    'true' if timer is paused, else 'false'
    */
    bool paused() const;

    /**
    *  @brief
    *    Start timer
    */
    void start();

    /**
    *  @brief
    *    Pause timer
    */
    void pause();

    /**
    *  @brief
    *    Stop timer
    */
    void stop();

    /**
    *  @brief
    *    Reset timer
    */
    void reset();


protected:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;


protected:
    bool m_paused;
    bool m_autoUpdate;

    time_point m_t0;
    time_point m_tp; // time_point of last pausing

    time_point m_t1;

    Duration m_offset;
    Duration m_elapsed;
};


} // namespace gloperate
