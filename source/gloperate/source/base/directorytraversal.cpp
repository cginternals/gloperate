
#include <gloperate/base/directorytraversal.h>

#include <globjects/base/File.h>
#include <globjects/NamedString.h>

#include <iozeug/directorytraversal.h>


namespace
{

#ifdef WIN32
    const std::string g_sep = "\\";
#elif __APPLE__
    const std::string g_sep = "/";
#else
    const std::string g_sep = "/";
#endif

bool endsWith(const std::string & str, const std::string & ending)
{
    return str.find_last_of(ending) == str.size() - 1;
}

}


namespace gloperate
{


void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recursive)
{
    iozeug::scanDirectory(directory, fileExtension, recursive, [](const std::string & file) {
        globjects::NamedString::create("/"+file, new globjects::File(file));
    });
}

void scanDirectory(const std::string & directory, const std::string & alias, const std::string & fileExtension, bool recursive)
{
    const auto sanitizedDirectory = removeTrailingPathSeparator(directory);
    const auto sanitizedAlias = removeTrailingPathSeparator(alias);

    iozeug::scanDirectory(sanitizedDirectory, fileExtension, recursive, [& sanitizedDirectory, & sanitizedAlias](const std::string & file) {
        globjects::NamedString::create("/"+sanitizedAlias+file.substr(sanitizedDirectory.size()), new globjects::File(file));
    });
}

std::string removeTrailingPathSeparator(const std::string & path)
{
    auto returnPath = path;

    while (returnPath.size() > 0 && (endsWith(returnPath, g_sep)))
    {
        returnPath = returnPath.substr(0, returnPath.size() - 1);
    }

    return returnPath;
}

std::string ensurePathSeparatorEnding(const std::string & path)
{
    return removeTrailingPathSeparator(path) + g_sep;
}


} // namespace globjectsutils
