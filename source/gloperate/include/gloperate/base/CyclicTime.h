
#pragma once


#include <memory>
#include <time.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


using longf = long double;


class ChronoTimer;


/**
*  @brief
*    Provides cyclic access control (in the range [0:1]) over a Timer
*
*    CyclicTime manages a osg::Timer and features an interface for floating time
*    in the closed interval [0;1] representing a full day and standard c 
*    time (time_t) simultaneously. The time updates have to be requested 
*    explicitly, thus simplifying usage between multiple recipients.
*    The time starts cycling automatically, but can also be paused, stopped, 
*    or set to a specific value.
*/
class GLOPERATE_API CyclicTime
{
public:
    static long systemUtcOffset();


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] time
    *    Initial time
    *  @param[in] secondsPerCycle
    *    Time after which the cycle restarts (in seconds)
    */
    CyclicTime(longf time = 0.0, longf secondsPerCycle = 0.0);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] time
    *    Initial time
    *  @param[in] utcOffset
    *    In seconds (UTC+01:00 is m_utcOffset = +3600)
    *  @param[in] secondsPerCycle
    *    Time after which the cycle restarts (in seconds)
    */
    CyclicTime(
        const time_t & time
    ,   const time_t & utcOffset
    ,   longf secondsPerCycle = 0.0);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~CyclicTime();

    /**
    *  @brief
    *    Increment time appropriate to secondsPerCycle
    */
    void update();

    // Cycling manipulation - does not tamper the time.

    /**
    *  @brief
    *    Start timer
    */
    void start(bool update = false);

    /**
    *  @brief
    *    Pause timer
    */
    void pause(bool update = false);

    /**
    *  @brief
    *    Reset timer
    *
    *    Resets the time to initial value (secondsPerCycle remain unchanged).
    */
    void reset(bool update = false);

    /**
    *  @brief
    *    Stop timer
    *
    *    Stops and resets the time.
    */
    void stop(bool update = false);

    longf secondsPerCycle() const;
    void setSecondsPerCycle(longf secondsPerCycle);

    /**
    *  @brief
    *    Get time
    *
    *  @return
    *    Float time in the interval [0;1]
    */
    longf timef() const;

    /**
    *  @brief
    *    Get time
    *
    *  @return
    *    Float time in the interval [0;1]
    */
    longf timef(bool updateFirst);

    /**
    *  @brief
    *    Set time
    *
    *    Sets only time, date remains unchanged.
    */
    void setTimef(longf time, bool update = false);

    /**
    *  @brief
    *    Get elapsed float time from initialized time
    */
    longf nonModTimef(bool update = false);

    /**
    *  @brief
    *    Get current time
    *
    *  @return
    *    Time in seconds from initial time.
    */
    time_t time() const;

    /**
    *  @brief
    *    Get current time
    *
    *  @return
    *    Time in seconds from initial time.
    */
    time_t time(bool update);
    void setTime(const time_t & time, bool update = false);

    time_t utcOffset() const;
    void setUtcOffset(const time_t & utcOffset /* in seconds */);

    bool isRunning() const;


protected:
    static longf secondsTof(const time_t & time);
    static time_t fToSeconds(longf time);

    void initialize();

    longf elapsed() const;


protected:
    enum class Status
    {
        Cycling
    ,   Pausing
    };


protected:
    std::unique_ptr<ChronoTimer> m_timer;

    longf m_secondsPerCycle;
    Status m_mode;
    longf m_offset;
    longf m_lastModeChangeTime;
    time_t m_utcOffset;

    time_t m_time[3]; // [2] is for stop
    longf m_timef[3]; // [2] is for stop
};


} // namespace gloperate
