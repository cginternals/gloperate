
#include <gloperate/viewer/Timer.h>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ViewerContext.h>
#include <gloperate/viewer/TimeManager.h>


namespace gloperate
{


Timer::Timer(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_active(false)
, m_singleShot(false)
, m_interval(0.0f)
, m_remaining(0.0f)
{
    m_viewerContext->timeManager()->registerTimer(this);
}

Timer::~Timer()
{
    stop();

    m_viewerContext->timeManager()->unregisterTimer(this);
}

bool Timer::isActive() const
{
    return m_active;
}

void Timer::start(float interval, bool singleShot)
{
    if (!m_active)
    {
        m_viewerContext->timeManager()->activateTimer();
    }

    m_active     = true;
    m_interval   = interval;
    m_remaining  = interval;
    m_singleShot = singleShot;
}

void Timer::stop()
{
    if (m_active)
    {
        m_viewerContext->timeManager()->deactivateTimer();
    }

    m_active     = false;
    m_interval   = 0.0f;
    m_remaining  = 0.0f;
    m_singleShot = false;
}

float Timer::interval() const
{
    return m_interval;
}

float Timer::remainingTime() const
{
    return m_remaining;
}

void Timer::update(float delta)
{
    if (m_active && delta >= 0.0f)
    {
        if (m_remaining > delta)
        {
            m_remaining -= delta;
        }
        else
        {
            // Timer elapsed
            onElapsed();

            // Reset timer
            if (m_singleShot)
            {
                m_active     = false;
                m_interval   = 0.0f;
                m_remaining  = 0.0f;
                m_singleShot = false;
            }
            else
            {
                m_remaining  = m_interval;
            }
        }
    }
}

void Timer::onElapsed()
{
    // Emit signal
    this->elapsed();
}


} // namespace gloperate
