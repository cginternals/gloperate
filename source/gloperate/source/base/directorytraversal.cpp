
#include <gloperate/base/directorytraversal.h>

#ifdef _MSC_VER
#include "windows.h"
#include "dirent_msvc.h"
#else
#include <dirent.h>
#endif

#include <globjects/base/File.h>
#include <globjects/logging.h>
#include <globjects/NamedString.h>


namespace
{

#ifdef WIN32
    const std::string g_sep = "\\";
#elif __APPLE__
    const std::string g_sep = "/";
#else
    const std::string g_sep = "/";
#endif


void getFiles(const std::string & dirName, bool recursive, std::vector<std::string> & files)
{
    DIR* dir = opendir(dirName.c_str());
    dirent* entry;
    if (!dir)
    {
        globjects::warning() << "Could not open directory " << dirName << ".";
    }
    else
    {
        while ((entry = readdir(dir)))
        {
            const std::string name = entry->d_name;
            const std::string path = dirName + "/" + name;

            bool isDir = false;
            bool isFile = false;

            if (entry->d_type == DT_DIR)
            {
                isDir = true;
            }
            else if (entry->d_type == DT_REG)
            {
                isFile = true;
            }
            else if (entry->d_type == DT_UNKNOWN)
            {
                DIR* subDir = opendir(path.c_str());
                isDir = (subDir != nullptr);
                isFile = !isDir;
                if (subDir != nullptr)
                    closedir(subDir);
            }

            assert(!(isDir && isFile));

            if (isDir)
            {
                if (name == "." || name == "..")
                {
                    continue;
                }

                if (recursive)
                {
                    getFiles(path, true, files);
                }
            }
            else if (isFile)
            {
                files.push_back(path);
            }
        }
        closedir(dir);
    }
}

std::vector<std::string> getFiles(const std::string & dirName, bool recursive)
{
    std::vector<std::string> files;

    getFiles(dirName, recursive, files);

    return files;
}

std::string getExtension(const std::string & filename)
{
    size_t pos = filename.find_last_of('.');

    if (pos == std::string::npos)
        return std::string();

    return filename.substr(pos+1);
}


} // namespace


using namespace globjects;


namespace gloperate
{


void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recursive)
{
    for (const std::string & file: getFiles(directory, recursive))
    {
        std::string extension = getExtension(file);

        if (fileExtension != "*" && extension != fileExtension)
            continue;

        NamedString::create("/"+file, new File(file));
    }
}


void ensurePathSeparatorEnding(std::string & path)
{
    auto endsWith = [] (const std::string & str, const std::string & ending) { return str.find_last_of(ending) == str.size() - 1; };

    while (path.size() > 0 && (endsWith(path, "/") || endsWith(path, "\\")))
    {
        path = path.substr(0, path.size() - 1);
    }

    path += g_sep;
}


} // namespace globjectsutils
