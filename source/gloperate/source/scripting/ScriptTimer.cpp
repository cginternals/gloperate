
#include <gloperate/scripting/ScriptTimer.h>

#include <vector>

#include <reflectionzeug/function/Function.h>
#include <reflectionzeug/variant/Variant.h>


namespace gloperate
{


ScriptTimer::ScriptTimer(ViewerContext * viewerContext, reflectionzeug::AbstractFunction * func)
: Timer(viewerContext)
, m_func(func)
{
}

ScriptTimer::~ScriptTimer()
{
    delete m_func;
}

void ScriptTimer::onElapsed()
{
    // Invoke function
    if (m_func) {
        std::vector<reflectionzeug::Variant> params;
        reflectionzeug::Variant res = m_func->call(params);
    }
}


} // namespace gloperate
