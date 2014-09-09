/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/plugin/PluginManager.h>
#include <gloperate/plugin/PluginLibrary.h>
#include <gloperate/plugin/Plugin.h>


#ifdef LINUX
    #include <dlfcn.h>

    class PluginLibraryImpl : public gloperate::PluginLibrary
    {
    public:
        PluginLibraryImpl(const std::string & filename)
        : gloperate::PluginLibrary(filename)
        , m_handle(0)
        {
            // Open library
            m_handle = dlopen(filename.c_str(), RTLD_NOW);
            if (m_handle) {
                // Get function pointers
                *reinterpret_cast<void**>(&m_initPluginPtr)      = dlsym(m_handle, "initPlugin");
                *reinterpret_cast<void**>(&m_getNumOfPluginsPtr) = dlsym(m_handle, "getNumOfPlugins");
                *reinterpret_cast<void**>(&m_getPluginPtr)       = dlsym(m_handle, "getPlugin");
                *reinterpret_cast<void**>(&m_deinitPluginPtr)    = dlsym(m_handle, "deinitPlugin");
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
#elif defined(WIN32)
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
            *reinterpret_cast<void**>(&m_initPluginPtr)      = GetProcAddress(m_dll, "initPlugin");
            *reinterpret_cast<void**>(&m_getNumOfPluginsPtr) = GetProcAddress(m_dll, "getNumOfPlugins");
            *reinterpret_cast<void**>(&m_getPluginPtr)       = GetProcAddress(m_dll, "getPlugin");
            *reinterpret_cast<void**>(&m_deinitPluginPtr)    = GetProcAddress(m_dll, "deinitPlugin");
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
    class PluginLibraryImpl : public gloperate::PluginLibrary
    {
    public:
        PluginLibraryImpl(const std::string & filename)
        : gloperate::PluginLibrary(filename)
        {
        }

        virtual ~PluginLibraryImpl() {
        }
    };
#endif


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
PluginManager::PluginManager()
{
}

/**
*  @brief
*    Destructor
*/
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

/**
*  @brief
*    Load plugin library
*/
void PluginManager::loadPluginLibrary(const std::string & filename)
{
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

/**
*  @brief
*    Get available plugins
*/
const std::vector<Plugin *> & PluginManager::plugins() const
{
    return m_plugins;
}

/**
*  @brief
*    Get plugin by name
*/
Plugin * PluginManager::plugin(const std::string & name) const
{
    // Get plugin
    if (m_pluginsByName.count(name) > 0) {
        return m_pluginsByName.at(name);
    }

    // Plugin not found
    return nullptr;
}


} // namespace gloperate
