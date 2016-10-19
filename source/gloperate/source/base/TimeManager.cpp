
#include <gloperate/base/TimeManager.h>

#include <algorithm>

#include <gloperate/base/Timer.h>
#include <gloperate/base/ScriptTimer.h>


// [TODO] Implement garbage collection of stopped scripting timers.
//
// At the moment, scripting timers are only deleted on shutdown.
// We can't delete timers directly when they are stopped, because the call to timer.stop()
// may be a result of the callback function itself, which hasn't returned, yet.
// To avoid deleting a function that is currently being called, we have to defer deletion.


namespace gloperate
{


TimeManager::TimeManager(Environment * environment)
: cppexpose::Object("timer")
, m_environment(environment)
, m_activeTimers(0)
, m_timeDelta(0.0f)
, m_nextId(1)
{
    // Register functions
    addFunction("start",    this, &TimeManager::scr_start);
    addFunction("once",     this, &TimeManager::scr_once);
    addFunction("stop",     this, &TimeManager::scr_stop);
    addFunction("stopAll",  this, &TimeManager::scr_stopAll);
    addFunction("nextTick", this, &TimeManager::scr_nextTick);
}

TimeManager::~TimeManager()
{
    // Delete timers
    for (auto it = m_scriptTimers.begin(); it != m_scriptTimers.end(); ++it)
    {
        delete it->second;
    }
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

int TimeManager::scr_start(int msec, const cppexpose::Variant & func)
{
    return startTimer(func, msec, false);
}

int TimeManager::scr_once(int msec, const cppexpose::Variant & func)
{
    return startTimer(func, msec, true);
}

void TimeManager::scr_stop(int id)
{
    // Check timer ID
    if (m_scriptTimers.count(id) < 1) {
        return;
    }

    // Stop timer
    ScriptTimer * timer = m_scriptTimers.at(id);
    timer->stop();
}

void TimeManager::scr_stopAll()
{
    // Enumerate all timers
    for (std::map<int, ScriptTimer *>::iterator it = m_scriptTimers.begin(); it != m_scriptTimers.end(); ++it)
    {
        // Stop timer
        ScriptTimer * timer = it->second;
        timer->stop();
    }
}

int TimeManager::scr_nextTick(const cppexpose::Variant & func)
{
    return startTimer(func, 0, true);
}

int TimeManager::startTimer(const cppexpose::Variant & func, int msec, bool singleShot)
{
    // Check if a function has been passed
    if (!func.hasType<cppexpose::Function>())
    {
        return -1;
    }

    // Get callback function
    cppexpose::Function function = func.value<cppexpose::Function>();

    // Create and start timer
    ScriptTimer * timer = new ScriptTimer(m_environment, function);
    timer->start(msec / 1000.0f, singleShot);

    // Store timer
    int id = m_nextId++;
    m_scriptTimers[id] = timer;

    // Return timer ID
    return id;
}


} // namespace gloperate
