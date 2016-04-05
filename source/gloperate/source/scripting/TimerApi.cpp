
#include <gloperate/scripting/TimerApi.h>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/Timer.h>
#include <gloperate/scripting/ScriptTimer.h>


// [TODO] Implement garbage collection of stopped timers.
//
// At the moment, timers are only deleted on shutdown.
// We can't delete timers directly when they are stopped, because the call to timer.stop()
// may be a result of the callback function itself, which hasn't returned, yet.
// To avoid deleting a function that is currently being called, we have to defer deletion.


namespace gloperate
{


TimerApi::TimerApi(ViewerContext * viewerContext)
: reflectionzeug::Object("timer")
, m_viewerContext(viewerContext)
, m_nextId(1)
{
    // Register functions
    addFunction("start",    this, &TimerApi::start);
    addFunction("once",     this, &TimerApi::once);
    addFunction("stop",     this, &TimerApi::stop);
    addFunction("stopAll",  this, &TimerApi::stopAll);
    addFunction("nextTick", this, &TimerApi::nextTick);
}

TimerApi::~TimerApi()
{
    // Delete timers
    for (std::map<int, ScriptTimer *>::iterator it = m_timers.begin(); it != m_timers.end(); ++it)
    {
        ScriptTimer * timer = it->second;
        delete timer;
    }
}

int TimerApi::start(int msec, const reflectionzeug::Variant & func)
{
    return startTimer(func, msec, false);
}

int TimerApi::once(int msec, const reflectionzeug::Variant & func)
{
    return startTimer(func, msec, true);
}

void TimerApi::stop(int id)
{
    // Check timer ID
    if (m_timers.count(id) < 1) {
        return;
    }

    // Stop timer
    ScriptTimer * timer = m_timers.at(id);
    timer->stop();
}

void TimerApi::stopAll()
{
    // Enumerate all timers
    for (std::map<int, ScriptTimer *>::iterator it = m_timers.begin(); it != m_timers.end(); ++it)
    {
        // Stop timer
        ScriptTimer * timer = it->second;
        timer->stop();
    }
}

int TimerApi::nextTick(const reflectionzeug::Variant & func)
{
    return startTimer(func, 0, true);
}

int TimerApi::startTimer(const reflectionzeug::Variant & func, int msec, bool singleShot)
{
    // Check if a function has been passed
    if (!func.canConvert<reflectionzeug::AbstractFunction*>())
    {
        return -1;
    }

    // Get callback function
    auto function = func.value<reflectionzeug::AbstractFunction*>();
    if (!function)
    {
        return -1;
    }

    // Make a persistent copy of the callback function
    auto functionCopy = function->clone();

    // Create and start timer
    ScriptTimer * timer = new ScriptTimer(m_viewerContext, functionCopy);
    timer->start(msec / 1000.0f, singleShot);

    // Store timer
    int id = m_nextId++;
    m_timers[id] = timer;

    // Return timer ID
    return id;
}


} // namespace gloperate
