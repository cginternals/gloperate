
#include <gloperate-glfw/Timer.h>


namespace gloperate_glfw
{


Timer::Timer(bool _start, bool autoUpdate)
:   m_paused(true)
,   m_auto(autoUpdate)
,   m_t0(clock::now())
,   m_tp(m_t0)
,   m_t1(m_t0)
,   m_offset(Duration::zero())
,   m_elapsed(Duration::zero())
{
    if(_start)
        start();
}

Timer::~Timer()
{
}

void Timer::update() const
{
    m_t1 = m_paused ? m_tp : clock::now();

    const auto delta = m_t1 - m_t0;  
    m_elapsed = delta + m_offset;
}

bool Timer::paused() const
{
    return m_paused;
}

void Timer::start()
{
    if(!m_paused)
        return;

    const time_point t2 = clock::now();
    m_offset -= t2 - m_tp;

    m_t1 = t2;
    m_paused = false;
}

void Timer::pause()
{
    if(m_paused)
        return;

    m_tp = clock::now();
    m_paused = true;
}

void Timer::stop()
{
    pause();
    reset();
}

void Timer::reset()
{
    m_offset = Duration::zero();
    m_elapsed = Duration::zero();

    m_t0 = clock::now();
    m_t1 = m_t0;
    m_tp = m_t0;
}

Timer::Duration Timer::elapsed() const
{
    if(m_auto)
        update();

    return m_elapsed;
}

void Timer::setAutoUpdating(const bool auto_update)
{
    m_auto = auto_update;
}

bool Timer::autoUpdating() const
{
    return m_auto;
}


} // namespace gloperate_glfw
