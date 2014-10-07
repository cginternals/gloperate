#include <gloperate/plugin/PluginManager.h>

#include <algorithm>

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <libgen.h>
#include <dirent.h>
#endif

#include <globjects/logging.h>

#include <gloperate/plugin/PluginLibrary.h>
#include <gloperate/plugin/Plugin.h>

#include "util/DirectoryIterator.h"


namespace
{
    // Define system specific filename properties
#ifdef WIN32
    const int RTLD_LAZY(0); // ignore for win32 - see dlopen
    const std::string g_sep = "\\";
    const std::string g_pre = "";
    const std::string g_ext = "dll";
#elif MAC_OS
    const std::string g_sep = "/";
    const std::string g_pre = "";
    const std::string g_ext = "dylib";
#else
    const std::string g_sep = "/";
    const std::string g_pre = "lib";
    const std::string g_ext = "so";
#endif

    class PluginLibraryImpl : public gloperate::PluginLibrary
    {
    public:
        PluginLibraryImpl(const std::string & filename)
            : gloperate::PluginLibrary(filename)
            , m_handle(0)
        {
            m_handle = dlopen(filename.c_str(), RTLD_LAZY);
            if (!m_handle)
            {
                globjects::warning() << dlerror();
                return;
            }

            *reinterpret_cast<void**>(&m_initPtr)       = dlsym(m_handle, "initialize");
            *reinterpret_cast<void**>(&m_numPluginsPtr) = dlsym(m_handle, "numPlugins");
            *reinterpret_cast<void**>(&m_pluginPtr)     = dlsym(m_handle, "plugin");
            *reinterpret_cast<void**>(&m_deinitPtr)     = dlsym(m_handle, "deinitialize");
        }

        virtual ~PluginLibraryImpl()
        {
            if (m_handle)
                dlclose(m_handle);
        }

    protected:

#ifdef WIN32
        // provide posix handles for windows funcs :P
        inline HMODULE dlopen(LPCSTR lpFileName, int ignore) { return LoadLibraryA(lpFileName); }
        inline FARPROC dlsym(HMODULE hModule, LPCSTR lpProcName) { return GetProcAddress(hModule, lpProcName); }
        inline    BOOL dlclose(HMODULE hModule) { return FreeLibrary(hModule); }
        inline   DWORD dlerror() { return GetLastError(); }

        HMODULE m_handle;
#else
        void * m_handle;
#endif
    };
}


namespace gloperate
{

std::string PluginManager::s_applicationPath = "";


void PluginManager::init(const std::string & applicationFilePath)
{
#ifdef WIN32
    // Set default plugin path to the path of the executable
    HMODULE appModule = GetModuleHandle(0);

    char szFilename[MAX_PATH];
    char szDrive[8];
    char szPath[MAX_PATH];

    if (GetModuleFileNameA(appModule, szFilename, MAX_PATH) > 0) 
    {
        _splitpath(szFilename, szDrive, szPath, NULL, NULL);
        s_applicationPath = std::string(szDrive) + std::string(szPath);
    }
#else
    // Save default plugin path
    s_applicationPath = dirname(const_cast<char *>(applicationFilePath.c_str()));
#endif

    s_applicationPath = DirectoryIterator::truncate(s_applicationPath);
}

PluginManager::PluginManager()
{
    m_paths.push_back(s_applicationPath);
}

PluginManager::~PluginManager()
{
    // note: The plugins do not need to (and must not) be destroyed, because this is done
    // inside the plugin library, when deinitialize() is called.

    for (const std::pair<std::string, PluginLibrary *> & i : m_librariesByFilePath)
        unloadLibrary(i.second);
}

void PluginManager::addPath(const std::string & path)
{
    const std::string p = DirectoryIterator::truncate(path);

    const std::vector<std::string>::const_iterator i = std::find(m_paths.cbegin(), m_paths.cend(), p);
    if (i == m_paths.end())
        return;

    m_paths.push_back(p);
}

void PluginManager::removePath(const std::string & path)
{
    const std::string p = DirectoryIterator::truncate(path);

    const std::vector<std::string>::const_iterator i = std::find(m_paths.cbegin(), m_paths.cend(), p);
    if (i == m_paths.end())
        return;

    m_paths.erase(i);
}

void PluginManager::setPaths(const std::vector<std::string> & paths)
{
    m_paths.clear();

    for (std::string path : paths)
        m_paths.push_back(DirectoryIterator::truncate(path));
}

const std::vector<std::string> & PluginManager::paths() const
{
    return m_paths;
}

void PluginManager::scan(const std::string & identifier, bool reload)
{
    const std::vector<std::string> files = DirectoryIterator::files(m_paths, true);

    for (const std::string & file : files)
    {
        // check if path meets search criteria
        if (DirectoryIterator::extension(file) != g_ext)
            continue;

        if (identifier.empty() || file.find(identifier, file.find_last_of(g_sep)) != std::string::npos)
            loadLibrary(file, reload);
    }
}

bool PluginManager::load(const std::string & name, const bool reload)
{
    const std::vector<std::string> files = DirectoryIterator::files(m_paths, true);

    // compose plugin file name, e.g., on linux: "/" + "lib" + name + ".so"
    const std::string fname = g_sep + g_pre + name + "." + g_ext;

    // search if this is in files
    for (const std::string & filePath : files)
    {
        if (filePath.find(fname) == filePath.npos)
            continue;

        return loadLibrary(filePath, reload);
    }    
    return false;
}

bool PluginManager::loadLibrary(const std::string & filePath, bool reload)
{
    auto i = m_librariesByFilePath.find(filePath);

    if (i != m_librariesByFilePath.cend() && !reload)
        return true;

    PluginLibrary * previous(nullptr);
    if (i != m_librariesByFilePath.cend())
        previous = i->second; // remember this, in case reloading fails

    PluginLibraryImpl * library = new PluginLibraryImpl(filePath);
    if (!library->isValid())
    {
        globjects::warning() << (previous ? "Reloading" : "loading") << " plugin(s) from '" << filePath << "' failed.";
        delete library;

        return false;
    }

    if (previous)
        unloadLibrary(previous);

    m_librariesByFilePath[filePath] = library; // in case of reload, this overwrites the previous

    library->initialize();

    // Iterate over plugins

    const unsigned int numPlugins = library->numPlugins();
    for (unsigned int i = 0; i < numPlugins; ++i) 
    {
        gloperate::Plugin * plugin = library->plugin(i);
        if (!plugin)
            continue;

        m_plugins.push_back(plugin);

        std::string name = plugin->name();
        m_pluginsByName[name] = plugin;
    }
    return true;
}

void PluginManager::unloadLibrary(PluginLibrary * library) const
{
    if (!library)
        return;

    library->deinitialize();
    delete library;
}

const std::vector<Plugin *> & PluginManager::plugins() const
{
    return m_plugins;
}

Plugin * PluginManager::plugin(const std::string & name) const
{
    if (m_pluginsByName.count(name) == 0)
        return nullptr;

    return m_pluginsByName.at(name);
}

void PluginManager::printPlugins() const
{
    for (Plugin * plugin : m_plugins) 
    {
        globjects::info() << " PLUGIN name: " << plugin->name() << " (" << plugin->type() << ")";
        globjects::info() << " description: " << plugin->description();
        globjects::info() << "     version: " << plugin->version();
        globjects::info() << "      vendor: "  << plugin->vendor();
        globjects::info();
    }
}

} // namespace gloperate
