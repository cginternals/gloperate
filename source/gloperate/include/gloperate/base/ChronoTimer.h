
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
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using Duration = std::chrono::duration<long long int, std::nano>;


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
    bool m_paused;      ///< Paused state
    bool m_autoUpdate;  ///< Auto update state

    time_point m_t0;    ///< Current time point
    time_point m_tp;    ///< Time point of last pause

    time_point m_t1;    ///< Ask @cgcostume

    Duration m_offset;  ///< Ask @cgcostume
    Duration m_elapsed; ///< Ask @cgcostume
};


} // namespace gloperate
