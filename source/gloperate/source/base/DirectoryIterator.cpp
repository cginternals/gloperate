
#include "DirectoryIterator.h"

#ifdef _MSC_VER
#include "windows.h"
#include "dirent_msvc.h"
#else
#include <dirent.h>
#endif

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
}


namespace gloperate
{


void DirectoryIterator::files(
    std::vector<std::string> & fpaths
,   const std::vector<std::string> & paths
,   bool prependpath
,   bool recursive)
{
    for (const std::string & path : paths)
    {
        const std::vector<std::string> fs(DirectoryIterator::files(path, prependpath, recursive));
        fpaths.insert(fpaths.end(), fs.begin(), fs.end());
    }
}

void DirectoryIterator::files(
    std::vector<std::string> & fpaths
,   const std::string & path
,   bool prependpath
,   bool recursive)
{
    DIR * dir = opendir(path.c_str());

    if (!dir)
    {
        globjects::warning() << "Open directory " << path << " failed.";
        return;
    }

    const std::string truncated = truncate(path);

    dirent * entry(nullptr);
    while ((entry = readdir(dir)))
    {
        const std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

        const std::string p = path + g_sep + name;

        bool isDir  = entry->d_type == DT_DIR;
        bool isFile = entry->d_type == DT_REG || entry->d_type == DT_LNK;

        if (entry->d_type == DT_UNKNOWN)
        {
            DIR * subdir = opendir(p.c_str());

            isDir  = (subdir != nullptr);
            isFile = !isDir;

            if (isDir)
                closedir(subdir);
        }

        if (isDir && recursive)
            files(fpaths, p, true);
        else if (isFile)
            fpaths.push_back(prependpath ? p : name);
    }

    closedir(dir);
}

std::vector<std::string> DirectoryIterator::files(
    const std::vector<std::string> & paths
,   bool prependpath
,   bool recursive)
{
    std::vector<std::string> fpaths;
    files(fpaths, paths, prependpath, recursive);

    return fpaths;
}

std::vector<std::string> DirectoryIterator::files(
    const std::string & path
,   bool prependpath
,   bool recursive)
{
    std::vector<std::string> fpaths;
    files(fpaths, path, prependpath, recursive);

    return fpaths;
}

std::string DirectoryIterator::extension(const std::string & filename)
{
    const size_t pos = filename.find_last_of('.');

    if (pos == std::string::npos)
        return std::string();

    return filename.substr(pos + 1);
}

std::string DirectoryIterator::truncate(const std::string & path)
{
    std::string truncated(path);
    while (truncated.size() > 0 && truncated.find_last_of(g_sep) == truncated.size() - 1)
        truncated = truncated.substr(0, truncated.find_last_of(g_sep));

    return truncated;
}


} // namespace gloperate
