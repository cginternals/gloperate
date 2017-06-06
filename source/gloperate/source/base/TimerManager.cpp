
#include <gloperate/base/TimerManager.h>

#include <algorithm>

#include <cppassist/memory/make_unique.h>


// [TODO] Implement garbage collection of stopped scripting timers.
//
// At the moment, scripting timers are only deleted on shutdown.
// We can't delete timers directly when they are stopped, because the call to timer.stop()
// may be a result of the callback function itself, which hasn't returned, yet.
// To avoid deleting a function that is currently being called, we have to defer deletion.


namespace gloperate
{


TimerManager::TimerManager(Environment * environment)
: cppexpose::Object("timer")
, m_environment(environment)
, m_nextId(1)
{
    // Register functions
    addFunction("start",    this, &TimerManager::scr_start);
    addFunction("once",     this, &TimerManager::scr_once);
    addFunction("stop",     this, &TimerManager::scr_stop);
    addFunction("stopAll",  this, &TimerManager::scr_stopAll);
    addFunction("nextTick", this, &TimerManager::scr_nextTick);
}

TimerManager::~TimerManager()
{
}

void TimerManager::update()
{
    auto duration = m_clock.elapsed();
    m_clock.reset();

    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();

    update(delta);
}

void TimerManager::update(float delta)
{
    for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
    {
        Timer * timer = it->second.get();

        if (timer->active)
        {
            updateTimer(timer, delta);
        }
    }
}

int TimerManager::scr_start(int msec, const cppexpose::Variant & func)
{
    return startTimer(func, msec, false);
}

int TimerManager::scr_once(int msec, const cppexpose::Variant & func)
{
    return startTimer(func, msec, true);
}

void TimerManager::scr_stop(int id)
{
    // Check timer ID
    if (m_timers.count(id) < 1) {
        return;
    }

    // Stop timer
    Timer * timer = m_timers.at(id).get();
    stopTimer(timer);
}

void TimerManager::scr_stopAll()
{
    // Enumerate all timers
    for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
    {
        // Stop timer
        Timer * timer = it->second.get();
        stopTimer(timer);
    }
}

int TimerManager::scr_nextTick(const cppexpose::Variant & func)
{
    return startTimer(func, 0, true);
}

int TimerManager::startTimer(const cppexpose::Variant & func, int msec, bool singleShot)
{
    // Check if a function has been passed
    if (!func.hasType<cppexpose::Function>())
    {
        return -1;
    }

    // Get callback function
    cppexpose::Function function = func.value<cppexpose::Function>();

    // Create and start timer
    auto timer = cppassist::make_unique<Timer>();
    timer->interval   = msec / 1000.0f;
    timer->remaining  = msec / 1000.0f;
    timer->singleShot = singleShot;
    timer->active     = true;
    timer->func       = function;

    // Store timer
    int id = m_nextId++;
    m_timers[id] = std::move(timer);

    // Return timer ID
    return id;
}

void TimerManager::stopTimer(Timer * timer)
{
    timer->active     = false;
    timer->interval   = 0.0f;
    timer->remaining  = 0.0f;
    timer->singleShot = false;
}

void TimerManager::updateTimer(Timer * timer, float delta)
{
    if (timer->active && delta >= 0.0f)
    {
        if (timer->remaining > delta)
        {
            timer->remaining -= delta;
        }
        else
        {
            // Timer elapsed
            std::vector<cppexpose::Variant> params;
            cppexpose::Variant res = timer->func.call(params);

            // Reset timer
            if (timer->singleShot)
            {
                timer->active     = false;
                timer->interval   = 0.0f;
                timer->remaining  = 0.0f;
                timer->singleShot = false;
            }
            else
            {
                timer->remaining = timer->interval;
            }
        }
    }
}


} // namespace gloperate
