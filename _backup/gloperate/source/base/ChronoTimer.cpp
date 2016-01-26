
#include <gloperate/base/ChronoTimer.h>


namespace gloperate
{


ChronoTimer::ChronoTimer(const bool _start, const bool autoUpdate)
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

ChronoTimer::~ChronoTimer()
{
}

void ChronoTimer::update() const
{
    m_t1 = m_paused ? m_tp : clock::now();

    const auto delta = m_t1 - m_t0;  
    m_elapsed = delta + m_offset;
}

bool ChronoTimer::paused() const
{
    return m_paused;
}

void ChronoTimer::start()
{
    if(!m_paused)
        return;

    const time_point t2 = clock::now();
    m_offset -= t2 - m_tp;

    m_t1 = t2;
    m_paused = false;
}

void ChronoTimer::pause()
{
    if(m_paused)
        return;

    m_tp = clock::now();
    m_paused = true;
}

void ChronoTimer::stop()
{
    pause();
    reset();
}

void ChronoTimer::reset()
{
    m_offset = Duration::zero();
    m_elapsed = Duration::zero();

    m_t0 = clock::now();
    m_t1 = m_t0;
    m_tp = m_t0;
}

ChronoTimer::Duration ChronoTimer::elapsed() const
{
    if(m_auto)
        update();

    return m_elapsed;
}

void ChronoTimer::setAutoUpdating(const bool auto_update)
{
    m_auto = auto_update;
}

bool ChronoTimer::autoUpdating() const
{
    return m_auto;
}


} // namespace gloperate
