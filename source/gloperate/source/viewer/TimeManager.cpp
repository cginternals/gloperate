
#include <gloperate/viewer/TimeManager.h>

#include <algorithm>

#include <gloperate/viewer/Timer.h>


namespace gloperate
{


TimeManager::TimeManager(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_activeTimers(0)
, m_timeDelta(0.0f)
{
}

TimeManager::~TimeManager()
{
}

bool TimeManager::update()
{
    auto duration = m_clock.elapsed();
    m_clock.reset();

    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();

    return update(delta);
}

bool TimeManager::update(float delta)
{
    m_timeDelta = delta;

    for (Timer * timer : m_timers)
    {
        if (timer->isActive())
        {
            timer->update(m_timeDelta);
        }
    }

    return m_activeTimers > 0;
}

float TimeManager::timeDelta() const
{
    return m_timeDelta;
}

void TimeManager::registerTimer(Timer * timer)
{
    m_timers.push_back(timer);
}

void TimeManager::unregisterTimer(Timer * timer)
{
    m_timers.erase(std::find(m_timers.begin(), m_timers.end(), timer));
}

void TimeManager::activateTimer()
{
    if (m_activeTimers == 0)
    {
        m_clock.reset();
    }

    m_activeTimers++;
}

void TimeManager::deactivateTimer()
{
    if (m_activeTimers > 0)
    {
        m_activeTimers--;
    }
}


} // namespace gloperate
