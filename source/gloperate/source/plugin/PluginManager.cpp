
#include <gloperate/plugin/PluginManager.h>

#include <algorithm>

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <libgen.h>
#include <dirent.h>
#endif

#include <iozeug/filename.h>

#include <reflectionzeug/variant/Variant.h>
#include <reflectionzeug/tools/SerializerJSON.h>

#include <globjects/logging.h>

#include <gloperate/plugin/PluginLibrary.h>
#include <gloperate/plugin/Plugin.h>

#include "base/DirectoryIterator.h"


namespace
{
    // Define system specific filename properties
#ifdef WIN32
    const int RTLD_LAZY(0); // ignore for win32 - see dlopen
    const std::string g_sep = "\\";
    const std::string g_pre = "";
    const std::string g_ext = "dll";
#elif __APPLE__
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


using namespace reflectionzeug;


namespace gloperate
{


PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
    // Note: The plugins do not need to (and must not) be destroyed, because this is done
    // inside the plugin library, when deinitialize() is called.

    // Unload plugin libraries
    for (const std::pair<std::string, PluginLibrary *> & it : m_librariesByFilePath) {
        unloadLibrary(it.second);
    }
}

const std::vector<std::string> & PluginManager::paths() const
{
    return m_paths;
}

void PluginManager::setPaths(const std::vector<std::string> & paths)
{
    m_paths.clear();

    for (std::string path : paths) {
        m_paths.push_back(DirectoryIterator::truncate(path));
    }
}

void PluginManager::addPath(const std::string & path)
{
    // Ignore empty path
    if (path.empty())
        return;

    // Remove slash
    const std::string p = DirectoryIterator::truncate(path);

    // Check if search path is already in the list
    const std::vector<std::string>::const_iterator it = std::find(m_paths.cbegin(), m_paths.cend(), p);
    if (it != m_paths.end()) {
        return;
    }

    // Add search path
    m_paths.push_back(p);
}

void PluginManager::removePath(const std::string & path)
{
    // Remove slash
    const std::string p = DirectoryIterator::truncate(path);

    // Check if search path is in the list
    const std::vector<std::string>::iterator it = std::find(m_paths.begin(), m_paths.end(), p);
    if (it == m_paths.end()) {
        return;
    }

    // Remove search path
    m_paths.erase(it);
}

void PluginManager::scan(const std::string & identifier, bool reload)
{
    // List all files in all search paths
    const std::vector<std::string> files = DirectoryIterator::files(m_paths, true);
    for (const std::string & file : files)
    {
        // Check if file is a library
        if (DirectoryIterator::extension(file) != g_ext)
            continue;

        // Check if library name corresponds to search criteria
        std::string query = identifier + "." + g_ext;
        if (identifier.empty() || file.find(query, file.find_last_of(g_sep)) != std::string::npos)
            loadLibrary(file, reload);
    }

    // Announce loaded plugins have changed
    pluginsChanged();
}

bool PluginManager::load(const std::string & filePath, const bool reload)
{
    // Load plugin library
    bool res = loadLibrary(filePath, reload);

    // Announce loaded plugins have changed
    pluginsChanged();

    return res;
}

const std::vector<Plugin *> & PluginManager::plugins() const
{
    // Return list of plugins
    return m_plugins;
}

Plugin * PluginManager::plugin(const std::string & name) const
{
    // Check if plugin exists
    if (m_pluginsByName.count(name) == 0) {
        return nullptr;
    }

    // Return plugin
    return m_pluginsByName.at(name);
}

void PluginManager::printPlugins() const
{
    // Print info about each plugin
    for (Plugin * plugin : m_plugins)
    {
        globjects::info() << " PLUGIN name: " << plugin->name() << " (" << plugin->type() << ")";
        globjects::info() << " description: " << plugin->description();
        globjects::info() << "     version: " << plugin->version();
        globjects::info() << "      vendor: " << plugin->vendor();
        globjects::info();
    }
}

bool PluginManager::loadLibrary(const std::string & filePath, bool reload)
{
    // Check if library is already loaded and reload is not requested
    auto it = m_librariesByFilePath.find(filePath);
    if (it != m_librariesByFilePath.cend() && !reload) {
        return true;
    }

    // Get path to directory containing the plugin library
    std::string dirPath = DirectoryIterator::truncate(iozeug::getPath(filePath));

    // Initialize plugin info
    std::string relDataPath = "";

    // Load extra information from "PluginInfo.json" if present
    Variant pluginInfo;
    SerializerJSON json;
    if (json.load(pluginInfo, dirPath + g_sep + "PluginInfo.json")) {
        // Read plugin info
        VariantMap & map = *(pluginInfo.asMap());
        if (map.count("relDataPath") > 0) {
            relDataPath = dirPath + g_sep + DirectoryIterator::truncate(map["relDataPath"].value<std::string>()) + g_sep;
        }
    }

    // If library was already loaded, remember it in case reloading fails
    PluginLibrary * previous = nullptr;
    if (it != m_librariesByFilePath.cend()) {
        previous = it->second;
    }

    // Open plugin library
    PluginLibraryImpl * library = new PluginLibraryImpl(filePath);
    if (!library->isValid())
    {
        // Loading failed. Destroy library object and return failure.
        globjects::warning() << (previous ? "Reloading" : "Loading") << " plugin(s) from '" << filePath << "' failed.";

        delete library;
        return false;
    }

    // Library has been loaded. Unload previous incarnation.
    if (previous) {
        unloadLibrary(previous);
    }

    // Add library to list (in case of reload, this overwrites the previous)
    m_librariesByFilePath[filePath] = library;

    // Initialize plugin library
    library->initialize();

    // Iterate over plugins
    const unsigned int numPlugins = library->numPlugins();
    for (unsigned int i = 0; i < numPlugins; ++i) 
    {
        // Get plugin
        gloperate::Plugin * plugin = library->plugin(i);
        if (!plugin)
            continue;

        // Set relative data path for plugin (if known)
        if (!relDataPath.empty()) {
            plugin->setRelDataPath(relDataPath.c_str());
        }

        // Add plugin to list
        m_plugins.push_back(plugin);

        // Save plugin by name
        std::string name = plugin->name();
        m_pluginsByName[name] = plugin;
    }

    // Return success
    return true;
}

void PluginManager::unloadLibrary(PluginLibrary * library)
{
    // Check parameters
    if (!library) {
        return;
    }

    // Unload plugin library
    library->deinitialize();
    delete library;
}


} // namespace gloperate
