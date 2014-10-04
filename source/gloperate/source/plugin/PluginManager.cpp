#include <gloperate/plugin/PluginManager.h>

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
            if (m_handle)
            {
                *reinterpret_cast<void**>(&m_initPtr)       = dlsym(m_handle, "initialize");
                *reinterpret_cast<void**>(&m_numPluginsPtr) = dlsym(m_handle, "numPlugins");
                *reinterpret_cast<void**>(&m_pluginPtr)     = dlsym(m_handle, "plugin");
                *reinterpret_cast<void**>(&m_deinitPtr)     = dlsym(m_handle, "deinitialize");
            }
            else
            {
                globjects::warning() << dlerror();
            }
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

std::string PluginManager::s_defaultScanPath = "";

void PluginManager::init(const std::string & executablePath)
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
        s_defaultScanPath = std::string(szDrive) + std::string(szPath);
    }
#else
    // Save default plugin path
    s_defaultScanPath = dirname(const_cast<char *>(executablePath.c_str()));
#endif
    s_defaultScanPath = DirectoryIterator::truncate(s_defaultScanPath);
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
    // Note: The plugins do not need to (and must not) be destroyed, because this is done
    // inside the plugin library, when deinitialize() is called.

    for (gloperate::PluginLibrary * library : m_libraries) 
    {
        library->deinitialize();
        delete library;
    }
}

std::string PluginManager::scanDirectory() const
{
    return m_scanPath;
}

void PluginManager::setScanDirectory(const std::string & path)
{
    m_scanPath = DirectoryIterator::truncate(path);
}

void PluginManager::scan(const std::string & identifier)
{
    std::string path = m_scanPath.empty() ? PluginManager::s_defaultScanPath : m_scanPath;

    const std::vector<std::string> files(DirectoryIterator::files(path));

    for (const std::string & file : files)
    {
        // check if path meets search criteria
        if (DirectoryIterator::extension(file) != g_ext)
            continue;

        if (identifier.empty() || file.find(identifier) != std::string::npos)
            loadLibrary(path + g_sep + file);
    }
}

void PluginManager::load(const std::string & name)
{
    std::string path = m_scanPath.empty() ? PluginManager::s_defaultScanPath : m_scanPath;

    // Try to load plugin - compose filename, e.g., on linux: path + "/" + "lib" + name + ".so"
    loadLibrary(path + g_sep + g_pre + name + "." + g_ext);
}

void PluginManager::loadLibrary(const std::string & filepath)
{
    PluginLibraryImpl * library = new PluginLibraryImpl(filepath);
    if (library->isValid())
    {
        m_libraries.push_back(library);

        library->initialize();

        // Iterate over plugins
        unsigned int numPlugins = library->numPlugins();
        for (unsigned int i = 0; i < numPlugins; ++i) 
        {
            gloperate::Plugin * plugin = library->plugin(i);
            if (plugin) 
            {
                m_plugins.push_back(plugin);

                std::string name = plugin->name();
                m_pluginsByName[name] = plugin;
            }
        }
    }
    else // error, close library
    {
        globjects::warning() << "Loading plugin(s) from '" << filepath << "' failed.";
        delete library;
    }    
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
