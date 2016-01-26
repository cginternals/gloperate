
#pragma once


#include <list>

#include <gloperate/ext-includes-begin.h>
#include <QTimer>
#include <gloperate/ext-includes-end.h>

#include <reflectionzeug/Object.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace reflectionzeug {
    class AbstractFunction;
}


namespace gloperate_qt {


/**
*  @brief
*    Timer functions for scripting interface
*/
class GLOPERATE_QT_API TimerApi : public reflectionzeug::Object
{
public:
    /**
    *  @brief
    *    Constructor
    */
    TimerApi();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TimerApi();

    /**
    *  @brief
    *    Promote a mainloop update to the scripting interface
    *
    *  @param[in] delta
    *    Time delta
    */
    void tickUpdate(float delta);

    // Timer API
    int  start(int msec, const reflectionzeug::Variant & func);
    int  once(int msec, const reflectionzeug::Variant & func);
    void stop(int id);
    void stopAll();
    void nextTick(const reflectionzeug::Variant & func);
    void clearNextTick();


protected:
    // Helper functions
    int findFreeTimer();


protected:
    /**
    *  @brief
    *    Timer that calls script-functions as callbacks
    */
    class ScriptTimer : public QTimer
    {
        public:
            ScriptTimer();
            virtual ~ScriptTimer();

            void clear();
            void addCallback(reflectionzeug::AbstractFunction * callback);

        protected:
            virtual void timerEvent(QTimerEvent * event);

        protected:
            std::list<reflectionzeug::AbstractFunction*> m_callbacks;
            std::list<reflectionzeug::AbstractFunction*> m_deletedCallbacks;
    };


protected:
    std::list<reflectionzeug::AbstractFunction*> m_tickCallbacks;       /**< List of next-tick callbacks */
    std::list<reflectionzeug::AbstractFunction*> m_deletedCallbacks;    /**< List of callbacks that have been deleted */
    ScriptTimer                                  m_timer[32];           /**< Scripting timers */
};


} // namespace gloperate_qt
