
#include <gloperate/base/CyclicTime.h>

#include <cmath>
#include <chrono>

#include <gloperate/ext-includes-begin.h>
#include <glm/common.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/base/ChronoTimer.h>


namespace gloperate
{


long CyclicTime::systemUtcOffset()
{
    long tz;

#ifdef __GNUC__
    tz = timezone;
#else // __GNUC__
    _get_timezone(&tz);
#endif // __GNUC__

    return tz;
}


CyclicTime::CyclicTime(
    const longf time
,   const longf secondsPerCycle)
:   m_timer(new ChronoTimer())
,   m_secondsPerCycle(secondsPerCycle)
,   m_mode(Status::Pausing)
,   m_offset(0.f)
,   m_lastModeChangeTime(0.f)
,   m_utcOffset(0)
{
    initialize();
    setTimef(time, true);
}

CyclicTime::CyclicTime(
    const time_t & time
,   const time_t & utcOffset
,   const longf secondsPerCycle)
:   m_timer(new ChronoTimer())
,   m_secondsPerCycle(secondsPerCycle)
,   m_mode(Status::Pausing)
,   m_offset(0.f)
,   m_lastModeChangeTime(0.f)
,   m_utcOffset(utcOffset)
{
    m_timer->setAutoUpdating(true);

    initialize();
    setTime(time, true);
}

void CyclicTime::initialize()
{
    using nanof = std::chrono::duration <longf, std::nano>;
    m_lastModeChangeTime = std::chrono::duration_cast<nanof>(m_timer->elapsed()).count();

    m_timef[0] = m_timef[1] = m_timef[2] = 0.f;
    m_time[0]  = m_time[1]  = m_time[2]  = 0;
}

CyclicTime::~CyclicTime()
{
}

longf CyclicTime::elapsed() const
{
    using secf = std::chrono::duration <longf>;
    return std::chrono::duration_cast<secf>(m_timer->elapsed()).count();
}

void CyclicTime::update()
{
    const longf elapsed = (Status::Cycling == m_mode ? this->elapsed() : m_lastModeChangeTime);

    const longf elapsedTimef = m_secondsPerCycle > 0.f ? elapsed / m_secondsPerCycle : 0.f;

    m_timef[1] = glm::fract(m_timef[0] + elapsedTimef + m_offset);

    m_time[1] = fToSeconds(elapsedTimef + m_offset) + static_cast<time_t>(m_time[0]);
}

longf CyclicTime::timef() const
{
    return m_timef[1];
}

longf CyclicTime::timef(const bool updateFirst)
{
    if (updateFirst)
        update();

    return m_timef[1];
}

time_t CyclicTime::time() const
{
    return m_time[1] + systemUtcOffset();
}

void CyclicTime::setTimef(longf timef, const bool forceUpdate)
{
    timef = glm::fract(timef);

    if (timef >= 1.f)
        timef = 0.f;

    m_timef[0] = timef;
    m_timef[2] = m_timef[0];

    m_offset = 0;

    const time_t seconds(fToSeconds(timef));

#ifdef __GNUC__
    struct tm lcl(*localtime(&m_time[1]));
#else // __GNUC__
    struct tm lcl;
    localtime_s(&lcl, &m_time[1]);
#endif // __GNUC__      

    lcl.tm_hour = static_cast<int>(seconds) / 3600;
    lcl.tm_min  = static_cast<int>(seconds) % 3600 / 60;
    lcl.tm_sec  = static_cast<int>(seconds) % 60;

    time_t mt = mktime(&lcl);
    if(mt == -1)
        m_time[0] = m_time[2] = 0;
    else
        m_time[0] = m_time[2] = mktime(&lcl) - systemUtcOffset();

    reset(forceUpdate);
}

longf CyclicTime::nonModTimef(const bool updateFirst)
{
    return secondsTof(time(updateFirst));
}

time_t CyclicTime::time(const bool updateFirst)
{
    if (updateFirst)
        update();

    return m_time[1] + systemUtcOffset();
}

void CyclicTime::setTime(
    const time_t & time
,   const bool forceUpdate)
{
    time_t t = time - systemUtcOffset();

    m_time[0] = t;
    m_time[2] = m_time[0];

    m_timef[0] = glm::fract(secondsTof(t));
    m_timef[2] = m_timef[0];

    m_offset = 0;

    reset(forceUpdate);
}

longf CyclicTime::secondsPerCycle() const 
{
    return m_secondsPerCycle; 
}

void CyclicTime::setSecondsPerCycle(const longf secsPerCycle)
{
    // intepret elapsed seconds within new cycle time
    const longf elapsed = (Status::Cycling == m_mode ? this->elapsed() : m_lastModeChangeTime);

    if(m_secondsPerCycle > 0.f)
        m_offset += elapsed / m_secondsPerCycle;

    m_lastModeChangeTime = 0;

    m_secondsPerCycle = secsPerCycle;
    m_timer->start();
}

inline longf CyclicTime::secondsTof(const time_t & time)
{
    return static_cast<longf>((time) / static_cast<longf>(60.0 * 60.0 * 24.0));
}

inline time_t CyclicTime::fToSeconds(const longf time)
{
    return static_cast<time_t>(time * 60.0 * 60.0 * 24.0 + 0.1);
}

bool CyclicTime::isRunning() const
{
    return Status::Cycling == m_mode;
}

void CyclicTime::start(bool forceUpdate)
{
    if (m_mode != Status::Pausing)
        return;

    if (m_secondsPerCycle > 0.f)
        m_offset -= (elapsed() - m_lastModeChangeTime) / m_secondsPerCycle;

    m_mode = Status::Cycling;

    if (forceUpdate)
        update();
}

void CyclicTime::pause(const bool forceUpdate)
{
    if (m_mode != Status::Cycling)
        return;

    m_lastModeChangeTime = elapsed();

    m_mode = Status::Pausing;

    if (forceUpdate)
        update();
}

void CyclicTime::reset(const bool forceUpdate)
{
    m_offset = 0.f;
    m_lastModeChangeTime = 0.f;

    m_timef[0] = m_timef[2];
    m_time[0] = m_time[2];

    m_timer.reset(new ChronoTimer());

    if (forceUpdate)
        update();
}

void CyclicTime::stop(const bool forceUpdate)
{
    pause();
    reset(forceUpdate);
}

time_t CyclicTime::utcOffset() const
{
    return m_utcOffset;
}

void CyclicTime::setUtcOffset(const time_t & utcOffset)
{
    m_utcOffset = utcOffset;
}


} // namespace gloperate
