
#include <gloperate-qt/scripting/TimerApi.h>


namespace gloperate_qt {


TimerApi::TimerApi()
: reflectionzeug::Object("timer")
{
    // Register functions
    this->addFunction("start",         this, &TimerApi::start);
    this->addFunction("startOnce",     this, &TimerApi::start);
    this->addFunction("once",          this, &TimerApi::once);
    this->addFunction("stop",          this, &TimerApi::stop);
    this->addFunction("stopAll",       this, &TimerApi::stopAll);
    this->addFunction("nextTick",      this, &TimerApi::nextTick);
    this->addFunction("clearNextTick", this, &TimerApi::clearNextTick);
}

TimerApi::~TimerApi()
{
    // Remove tick callbacks
    clearNextTick();

    // Destroy former callback functions
    for (std::list<reflectionzeug::AbstractFunction*>::iterator it = m_deletedCallbacks.begin(); it != m_deletedCallbacks.end(); ++it) {
        reflectionzeug::AbstractFunction * func = *it;
        delete func;
    }
}

void TimerApi::tickUpdate(float delta)
{
    // For each connected callback function
    for (std::list<reflectionzeug::AbstractFunction*>::iterator it = m_tickCallbacks.begin(); it != m_tickCallbacks.end(); ++it) {
        // Get function
        reflectionzeug::AbstractFunction * func = *it;

        // Pass time delta to function
        std::vector<reflectionzeug::Variant> params;
        params.push_back(delta);

        // Invoke function
        reflectionzeug::Variant res = func->call(params);
    }
}

int TimerApi::start(int msec, const reflectionzeug::Variant & func)
{
    // Check if a function has been passed
    if (func.canConvert<reflectionzeug::AbstractFunction*>()) {
        // Find a free timer
        int id = findFreeTimer();
        if (id >= 0) {
            // Reset timer callbacks
            m_timer[id].clear();

            // Get callback function and make a persistent copy
            reflectionzeug::AbstractFunction * function = func.value<reflectionzeug::AbstractFunction*>()->clone();

            // Add callback
            m_timer[id].addCallback(function);

            // Start timer
            m_timer[id].setSingleShot(false);
            m_timer[id].start(msec);
            return id;
        }
    }

    // Could not activate timer
    return -1;
}

int TimerApi::once(int msec, const reflectionzeug::Variant & func)
{
    // Check if a function has been passed
    if (func.canConvert<reflectionzeug::AbstractFunction*>()) {
        // Find a free timer
        int id = findFreeTimer();
        if (id >= 0) {
            // Reset timer callbacks
            m_timer[id].clear();

            // Get callback function and make a persistent copy
            reflectionzeug::AbstractFunction * function = func.value<reflectionzeug::AbstractFunction*>()->clone();

            // Add callback
            m_timer[id].addCallback(function);

            // Start timer
            m_timer[id].setSingleShot(true);
            m_timer[id].start(msec);
            return id;
        }
    }

    // Could not activate timer
    return -1;
}

void TimerApi::stop(int id)
{
    // Deactivate timer
    if (id >= 0 && id < 32) {
        m_timer[id].stop();
        m_timer[id].clear();
    }
}

void TimerApi::stopAll()
{
    for (int i=0; i<32; i++) {
        m_timer[i].stop();
        m_timer[i].clear();
    }
}

void TimerApi::nextTick(const reflectionzeug::Variant & func)
{
    // Check if a function has been passed
    if (func.canConvert<reflectionzeug::AbstractFunction*>()) {
        // Get callback function and make a persistent copy
        reflectionzeug::AbstractFunction * function = func.value<reflectionzeug::AbstractFunction*>()->clone();

        // Add function to list of callbacks
        m_tickCallbacks.push_back(function);
    }
}

void TimerApi::clearNextTick()
{
    // Add functions to deleted-functions-list
    for (std::list<reflectionzeug::AbstractFunction*>::iterator it = m_tickCallbacks.begin(); it != m_tickCallbacks.end(); ++it) {
        m_deletedCallbacks.push_back(*it);
    }
    m_tickCallbacks.clear();
}

int TimerApi::findFreeTimer()
{
    // Find a timer that is not active
    for (int i=0; i<32; i++) {
        if (!m_timer[i].isActive())
            return i;
    }

    // No inactive timer found
    return -1;
}


// ScriptTimer implementation
TimerApi::ScriptTimer::ScriptTimer()
{
}

TimerApi::ScriptTimer::~ScriptTimer()
{
    // Destroy former callback functions
    for (std::list<reflectionzeug::AbstractFunction*>::iterator it = m_deletedCallbacks.begin(); it != m_deletedCallbacks.end(); ++it) {
        reflectionzeug::AbstractFunction * func = *it;
        delete func;
    }
}

void TimerApi::ScriptTimer::clear()
{
    // Put callback functions into deleted-functions-list. We can't delete the functions directly, because the call to
    // timer.clear() may be a result of the callback function itself, which hasn't returned, yet. To avoid deleting a
    // function that is currently being called, we have to defer the deletion
    for (std::list<reflectionzeug::AbstractFunction*>::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
        reflectionzeug::AbstractFunction * func = *it;
        m_deletedCallbacks.push_back(func);
    }
    m_callbacks.clear();
}

void TimerApi::ScriptTimer::addCallback(reflectionzeug::AbstractFunction * callback)
{
    m_callbacks.push_back(callback);
}

void TimerApi::ScriptTimer::timerEvent(QTimerEvent * event)
{
    QTimer::timerEvent(event);

    // For each connected callback function
    std::list<reflectionzeug::AbstractFunction*> callbacks = m_callbacks;
    for (std::list<reflectionzeug::AbstractFunction*>::iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
        // Get function
        reflectionzeug::AbstractFunction * func = *it;

        // Invoke function
        std::vector<reflectionzeug::Variant> params;
        reflectionzeug::Variant res = func->call(params);
    }
}


} // namespace gloperate_qt
