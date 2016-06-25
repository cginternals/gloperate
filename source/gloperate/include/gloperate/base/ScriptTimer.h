
#pragma once


#include <cppexpose/function/Function.h>

#include <gloperate/base/Timer.h>


namespace gloperate
{


class Environment;


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
    *  @param[in] environment
    *    Environment (must NOT be null!)
    *  @param[in] func
    *    Callback function
    */
    ScriptTimer(Environment * environment, cppexpose::Function func);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptTimer();


protected:
    virtual void onElapsed() override;


protected:
    cppexpose::Function m_func;  ///< Script function that is called when timer elapses
};


} // namespace gloperate
