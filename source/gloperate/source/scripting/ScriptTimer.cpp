
#include <gloperate/scripting/ScriptTimer.h>

#include <vector>

#include <cppexpose/function/AbstractFunction.h>
#include <cppexpose/variant/Variant.h>


namespace gloperate
{


ScriptTimer::ScriptTimer(ViewerContext * viewerContext, cppexpose::AbstractFunction * func)
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
        std::vector<cppexpose::Variant> params;
        cppexpose::Variant res = m_func->call(params);
    }
}


} // namespace gloperate
