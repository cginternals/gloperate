
#include <gloperate/scripting/SystemApi.h>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


SystemApi::SystemApi(ViewerContext * viewerContext)
: reflectionzeug::Object("system")
, m_viewerContext(viewerContext)
{
    // Register functions
    addFunction("exit", this, &SystemApi::exit);
}

SystemApi::~SystemApi()
{
}

void SystemApi::exit(int exitCode)
{
    m_viewerContext->exit(exitCode);
}


} // namespace gloperate
