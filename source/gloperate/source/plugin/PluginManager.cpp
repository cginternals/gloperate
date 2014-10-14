#include <gloperate/plugin/PluginManager.h>
#include <globjects/logging.h>
#include <gloperate/plugin/PluginLibrary.h>
#include <gloperate/plugin/Plugin.h>


#ifdef WIN32
    // Define system specific filename properties
    static const std::string g_sep = "\\";
    static const std::string g_pre = "";
    static const std::string g_ext = ".dll";

    // Implementation for Windows
    #include <Windows.h>

    class PluginLibraryImpl : public gloperate::PluginLibrary
    {
    public:
        PluginLibraryImpl(const std::string & filename)
            : gloperate::PluginLibrary(filename)
            , m_dll(0)
        {
            // Open library
            m_dll = LoadLibraryA(filename.c_str());
            if (m_dll) {
                // Get function pointers
                *reinterpret_cast<void**>(&m_initPluginPtr)      = GetProcAddress(m_dll, "initPluginLibrary");
                *reinterpret_cast<void**>(&m_getNumOfPluginsPtr) = GetProcAddress(m_dll, "getNumOfPlugins");
                *reinterpret_cast<void**>(&m_getPluginPtr)       = GetProcAddress(m_dll, "getPlugin");
                *reinterpret_cast<void**>(&m_deinitPluginPtr)    = GetProcAddress(m_dll, "deinitPluginLibrary");
            }
        }

        virtual ~PluginLibraryImpl() {
            // Close library
            if (m_dll) {
                FreeLibrary(m_dll);
            }
        }

    protected:
        HMODULE m_dll;
    };
#else
    #ifdef MAC_OS
        // Define system specific filename properties
        static const std::string g_sep = "/";
        static const std::string g_pre = "";
        static const std::string g_ext = ".dylib";
    #else
        // Define system specific filename properties
        static const std::string g_sep = "/";
        static const std::string g_pre = "lib";
        static const std::string g_ext = ".so";
    #endif

    // Implementation for POSIX systems
    #include <dlfcn.h>
    #include <libgen.h>
    #include <dirent.h>

    class PluginLibraryImpl : public gloperate::PluginLibrary
    {
    public:
        PluginLibraryImpl(const std::string & filename)
        : gloperate::PluginLibrary(filename)
        , m_handle(0)
        {
            // Open library
            m_handle = dlopen(filename.c_str(), RTLD_LAZY);
            if (m_handle) {
                // Get function pointers
                *reinterpret_cast<void**>(&m_initPluginPtr)      = dlsym(m_handle, "initPluginLibrary");
                *reinterpret_cast<void**>(&m_getNumOfPluginsPtr) = dlsym(m_handle, "getNumOfPlugins");
                *reinterpret_cast<void**>(&m_getPluginPtr)       = dlsym(m_handle, "getPlugin");
                *reinterpret_cast<void**>(&m_deinitPluginPtr)    = dlsym(m_handle, "deinitPluginLibrary");
            } else {
                globjects::debug() << dlerror();
            }
        }

        virtual ~PluginLibraryImpl() {
            // Close library
            if (m_handle) {
                dlclose(m_handle);
            }
        }

    protected:
        void * m_handle;
    };
#endif


namespace gloperate
{


std::string PluginManager::s_defaultPluginPath = "";


void PluginManager::init(const std::string & executablePath)
{
    #ifdef WIN32
        // Set default plugin path to the path of the executable
        HMODULE appModule = GetModuleHandle(0);
        char szFilename[MAX_PATH];
        char szDrive[8];
        char szPath[MAX_PATH];
        if (GetModuleFileNameA(appModule, szFilename, MAX_PATH) > 0) {
            _splitpath(szFilename, szDrive, szPath, NULL, NULL);
            PluginManager::s_defaultPluginPath = std::string(szDrive) + std::string(szPath);
        }
    #else
        // Save default plugin path
        PluginManager::s_defaultPluginPath = dirname(const_cast<char *>(executablePath.c_str()));
    #endif

    // Print default search path
    globjects::info() << "Default plugin path: " << PluginManager::s_defaultPluginPath;
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
    // Note: The plugins do not need to (and must not) be destroyed, because this is done
    // inside the plugin library, when deinitialize() is called.

    // Close libraries
    for (gloperate::PluginLibrary * library : m_libraries) {
        library->deinitialize();
        delete library;
    }
}

std::string PluginManager::scanDirectory() const
{
    return m_scanDirectory;
}

void PluginManager::setScanDirectory(const std::string & path)
{
    m_scanDirectory = path;
}

void PluginManager::scan(const std::string & identifier)
{
    // Get search directory
    std::string path = m_scanDirectory.empty() ? PluginManager::s_defaultPluginPath : m_scanDirectory;

#ifndef WIN32
    // Search for plugins
    DIR * dir = opendir(path.c_str());
    if (dir) {
        // Read dir entries
        dirent * entry = readdir(dir);
        while (entry) {
            // Get filename
            std::string name = entry->d_name;

            // Check if filename meets search criteria
            bool isLibrary = (g_ext.size() <= name.size() && std::equal(g_ext.rbegin(), g_ext.rend(), name.rbegin()));
            bool accepted  = (identifier.empty() || name.find(identifier) != std::string::npos);
            if (isLibrary && accepted) {
                // Try to load plugin
                loadLibrary(path + g_sep + name);
            }

            // Read next dir entry
            entry = readdir(dir);
        }
        closedir(dir);
    }
#else
    // [TODO] Windows implementation
#endif
}

void PluginManager::load(const std::string & name)
{
    // Get search directory
    std::string path = m_scanDirectory.empty() ? PluginManager::s_defaultPluginPath : m_scanDirectory;

    // Try to load plugin
    // Compose filename, e.g., on linux: path + "/" + "lib" + name + ".so"
    loadLibrary(path + g_sep + g_pre + name + g_ext);
}

void PluginManager::loadLibrary(const std::string & filename)
{
    globjects::info() << "Loading plugin '" << filename << "'";

    PluginLibraryImpl * library = new PluginLibraryImpl(filename);
    if (library->isValid()) {
        // Add library
        m_libraries.push_back(library);

        // Initialize library
        library->initialize();

        // Iterate over plugins
        unsigned int numPlugins = library->getNumOfPlugins();
        for (unsigned int i=0; i<numPlugins; i++) {
            // Get plugin
            gloperate::Plugin * plugin = library->getPlugin(i);
            if (plugin) {
                // Add plugin
                m_plugins.push_back(plugin);
                std::string name = plugin->name();
                m_pluginsByName[name] = plugin;
            }
        }
    } else {
        // Error, close library
        delete library;
    }
}

const std::vector<Plugin *> & PluginManager::plugins() const
{
    return m_plugins;
}

Plugin * PluginManager::plugin(const std::string & name) const
{
    // Get plugin
    if (m_pluginsByName.count(name) > 0) {
        return m_pluginsByName.at(name);
    }

    // Plugin not found
    return nullptr;
}

void PluginManager::printPlugins() const
{
    for (Plugin * plugin : m_plugins) {
        globjects::info() << "Plugin '" << plugin->name() << "' (" << plugin->type() << ")";
        globjects::info() << "  by " << plugin->vendor() << "";
        globjects::info() << "  " << plugin->description() << "";
        globjects::info() << "";
    }
}


} // namespace gloperate
