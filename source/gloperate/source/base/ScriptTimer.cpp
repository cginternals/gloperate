
#include <gloperate/base/ScriptTimer.h>

#include <vector>

#include <cppexpose/variant/Variant.h>


namespace gloperate
{


ScriptTimer::ScriptTimer(Environment * environment, cppexpose::Function func)
: Timer(environment)
, m_func(func)
{
}

ScriptTimer::~ScriptTimer()
{
}

void ScriptTimer::onElapsed()
{
    // Invoke function
    std::vector<cppexpose::Variant> params;
    cppexpose::Variant res = m_func.call(params);
}


} // namespace gloperate
