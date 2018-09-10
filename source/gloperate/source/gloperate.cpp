
#include <gloperate/gloperate.h>

#include <cppassist/fs/FilePath.h>

#include <iostream>

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

std::string determinePluginPath()
{
    std::string path = cppassist::FilePath(cpplocate::getLibraryPath(reinterpret_cast<void *>(&gloperate::dataPath))).directoryPath();
    path = cppassist::FilePath(path).path();

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

    std::cout << "Plugin path: " << path << std::endl;

    return path;
}


} // namespace gloperate
