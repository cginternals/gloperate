
#include <gloperate/scripting/TimerApi.h>

#include <globjects/base/baselogging.h>


namespace gloperate
{


TimerApi::TimerApi() : reflectionzeug::Object("timer")
{
    // Register functions
//  addFunction("sayHello", this, &TestApi::sayHello);
}

TimerApi::~TimerApi()
{
}


} // namespace gloperate
