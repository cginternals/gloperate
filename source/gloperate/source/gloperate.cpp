
#include <gloperate/gloperate.h>

#include <cpplocate/cpplocate.h>
#include <cpplocate/ModuleInfo.h>


namespace
{

std::string determineDataPath()
{
    const cpplocate::ModuleInfo moduleInfo = cpplocate::findModule("gloperate");
    const std::string moduleInfoPath = moduleInfo.value("dataPath");

    return moduleInfoPath.empty() ? "data" : moduleInfoPath;
}

std::string determinePluginPath()
{
    const cpplocate::ModuleInfo moduleInfo = cpplocate::findModule("gloperate");
    const std::string moduleInfoPath = moduleInfo.value("pluginPath");

    return moduleInfoPath.empty() ? "" : moduleInfoPath;
}

} // namespace


namespace gloperate
{


const std::string & dataPath()
{
    static const auto path = determineDataPath();

    return path;
}

const std::string & pluginPath()
{
    static const auto path = determinePluginPath();

    return path;
}


} // namespace gloperate
