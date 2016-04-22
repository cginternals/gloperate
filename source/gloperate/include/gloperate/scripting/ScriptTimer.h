
#pragma once


#include <gloperate/viewer/Timer.h>


namespace cppexpose
{
    class AbstractFunction;
}


namespace gloperate
{


class ViewerContext;


/**
*  @brief
*    Timer that executes a scripting function
*/
class ScriptTimer : public Timer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context (must NOT be null!)
    *  @param[in] func
    *    Callback function
    */
    ScriptTimer(ViewerContext * viewerContext, cppexpose::AbstractFunction * func);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptTimer();


protected:
    virtual void onElapsed() override;


protected:
    cppexpose::AbstractFunction * m_func;  ///< Script function that is called when timer elapses
};


} // namespace gloperate
