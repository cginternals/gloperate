
#include <gloperate/gloperate.h>

#include <cpplocate/cpplocate.h>


namespace
{

std::string determineDataPath()
{
    std::string path = cpplocate::locatePath("data/gloperate", "share/gloperate", reinterpret_cast<void *>(&gloperate::dataPath));
    if (path.empty()) path = "./data";
    else              path = path + "/data";

    return path;
}

std::string getDirectoryPath(const std::string & fullpath)
{
    if (fullpath.empty())
    {
        return "";
    }

    auto pos           = fullpath.rfind("/");
    const auto posBack = fullpath.rfind("\\");

    if (pos == std::string::npos || (posBack != std::string::npos && posBack > pos))
    {
        pos = posBack;
    }

    return fullpath.substr(0, pos);
}

std::string determinePluginPath()
{
    std::string path = getDirectoryPath(cpplocate::getLibraryPath(reinterpret_cast<void *>(&gloperate::dataPath)));

    return path;
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
