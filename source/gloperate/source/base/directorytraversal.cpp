
#include <gloperate/base/directorytraversal.h>

#include <globjects/base/File.h>
#include <globjects/NamedString.h>

#include <iozeug/directorytraversal.h>

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
    const auto sanitizedDirectory = directory.back() == '/' ? directory.substr(0, directory.size() - 1) : directory;
    const auto sanitizedAlias = alias.back() == '/' ? alias.substr(0, alias.size() - 1) : alias;

    iozeug::scanDirectory(sanitizedDirectory, fileExtension, recursive, [& sanitizedDirectory, & sanitizedAlias](const std::string & file) {
        globjects::NamedString::create("/"+sanitizedAlias+file.substr(sanitizedDirectory.size()), new globjects::File(file));
    });
}


} // namespace globjectsutils
