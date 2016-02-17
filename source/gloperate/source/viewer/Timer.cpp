
#include <gloperate/viewer/Timer.h>


namespace gloperate
{


Timer::Timer(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_active(false)
, m_singleShot(false)
, m_interval(0)
, m_remaining(0)
{
}

Timer::~Timer()
{
}

bool Timer::isActive() const
{
    return m_active;
}

void Timer::start(unsigned int interval, bool singleShot)
{
    m_active     = true;
    m_interval   = interval;
    m_remaining  = interval;
    m_singleShot = singleShot;
}

void Timer::stop()
{
    m_active     = false;
    m_interval   = 0;
    m_remaining  = 0;
    m_singleShot = false;
}

unsigned int Timer::interval() const
{
    return m_interval;
}

unsigned int Timer::remainingTime() const
{
    return m_remaining;
}

void Timer::update(unsigned int delta)
{
    if (m_active)
    {
        if (m_remaining > delta)
        {
            m_remaining -= delta;
        }
        else
        {
            // Timer fired
            this->fired();

            // Reset timer
            if (m_singleShot)
            {
                m_active     = false;
                m_interval   = 0;
                m_remaining  = 0;
                m_singleShot = false;
            }
            else
            {
                m_remaining  = m_interval;
            }
        }
    }
}


} // namespace gloperate
