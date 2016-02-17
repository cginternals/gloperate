
#include <gloperate/viewer/TimeManager.h>

#include <algorithm>

#include <gloperate/viewer/Timer.h>


namespace gloperate
{


TimeManager::TimeManager(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::update(unsigned int delta)
{
    for (Timer * timer : m_timers)
    {
        timer->update(delta);
    }
}

void TimeManager::registerTimer(Timer * timer)
{
    m_timers.push_back(timer);
}

void TimeManager::unregisterTimer(Timer * timer)
{
    m_timers.erase(std::find(m_timers.begin(), m_timers.end(), timer));
}


} // namespace gloperate
