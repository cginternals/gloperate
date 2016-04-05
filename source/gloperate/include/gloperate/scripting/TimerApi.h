
#pragma once


#include <map>

#include <reflectionzeug/Object.h>

#include <gloperate/gloperate_api.h>


namespace reflectionzeug {
    class Variant;
}


namespace gloperate
{


class ViewerContext;
class ScriptTimer;


/**
*  @brief
*    Scripting API for accessing timer functions
*/
class GLOPERATE_API TimerApi : public reflectionzeug::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    */
    TimerApi(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    ~TimerApi();


protected:
    // Scripting functions
    int  start(int msec, const reflectionzeug::Variant & func);
    int  once(int msec, const reflectionzeug::Variant & func);
    void stop(int id);
    void stopAll();
    int  nextTick(const reflectionzeug::Variant & func);

    // Helper functions
    int startTimer(const reflectionzeug::Variant & func, int msec, bool singleShot);


protected:
    ViewerContext                * m_viewerContext; ///< Viewer context (must NOT be null!)
    std::map<int, ScriptTimer *>   m_timers;        ///< List of activated timers
    int                            m_nextId;        ///< Next timer ID
};


} // namespace gloperate
