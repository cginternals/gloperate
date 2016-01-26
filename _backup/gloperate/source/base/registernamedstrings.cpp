
#include <gloperate/base/registernamedstrings.h>

#include <globjects/base/File.h>
#include <globjects/NamedString.h>

#include <iozeug/directorytraversal.h>
#include <iozeug/filename.h>


namespace gloperate
{


void registerNamedStrings(const std::string & directory, const std::string & fileExtension, bool recursive)
{
    iozeug::scanDirectory(directory, fileExtension, recursive, [](const std::string & file) {
        globjects::NamedString::create("/"+file, new globjects::File(file, false));
    });
}

void registerNamedStrings(const std::string & directory, const std::string & alias, const std::string & fileExtension, bool recursive)
{
    const auto sanitizedDirectory = iozeug::removeTrailingPathSeparator(directory);
    const auto sanitizedAlias = iozeug::removeTrailingPathSeparator(alias);

    iozeug::scanDirectory(sanitizedDirectory, fileExtension, recursive, [& sanitizedDirectory, & sanitizedAlias](const std::string & file) {
        globjects::NamedString::create("/"+sanitizedAlias+file.substr(sanitizedDirectory.size()), new globjects::File(file, false));
    });
}


} // namespace gloperate
