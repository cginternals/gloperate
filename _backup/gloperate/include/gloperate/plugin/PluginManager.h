
#pragma once


#include <string>
#include <set>
#include <vector>
#include <map>

#include <signalzeug/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class PluginLibrary;
class Plugin;


/**
*  @brief
*    Class for loading and managing plugins
*/
class GLOPERATE_API PluginManager
{
public:
    signalzeug::Signal<> pluginsChanged;


public:
    /**
    *  @brief
    *    Constructor
    */
    PluginManager();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PluginManager();

    /**
    *  @brief
    *    Get plugin search paths
    *
    *  @return
    *    List of paths that are searched for plugins
    */
    const std::vector<std::string> & searchPaths() const;

    /**
    *  @brief
    *    Set plugin search paths
    *
    *  @param[in] paths
    *    List of paths that are searched for plugins
    */
    void setSearchPaths(const std::vector<std::string> & paths);

    /**
    *  @brief
    *    Add plugin search path
    *
    *  @param[in] path
    *    Plugin search path
    *
    *  @remarks
    *    If path is empty or already in the path list, the path list is not changed.
    *    If no plugin path is provided, the default application path is used.
    */
    void addSearchPath(const std::string & path);

    /**
    *  @brief
    *    Remove plugin search path
    *
    *  @param[in] path
    *    Plugin search path
    */
    void removeSearchPath(const std::string & path);

    /**
    *  @brief
    *    Scan for plugins and load all found ones
    *
    *  @param[in] identifier
    *    Load only plugins with the given identifier as a substring
    *  @param[in] reload
    *    Reload plugin libraries that are already loaded?
    */
    void scan(const std::string & identifier = "", bool reload = false);

    /**
    *  @brief
    *    Load plugin library
    *
    *  @param[in] filePath
    *    Path to the plugin library
    *  @param[in] reload
    *    Reload plugin libraries that are already loaded?
    *
    *  @return
    *    'true' if library has been loaded successfully, else 'false'
    */
    bool load(const std::string & filePath, bool reload = true);

    /**
    *  @brief
    *    Get list of loaded plugins
    *
    *  @return
    *    List of plugins
    */
    const std::vector<Plugin *> & plugins() const;

    /**
    *  @brief
    *    Get list of loaded plugin libraries
    *
    *  @return
    *    List of plugin libraries
    */
    const std::vector<PluginLibrary *> pluginLibraries() const;

    /**
    *  @brief
    *    Get plugin by name
    *
    *  @param[in] name
    *    Plugin name
    *
    *  @return
    *    Pointer to the plugin, nullptr if no plugin with that name exists
    */
    Plugin * plugin(const std::string & name) const;

    /**
    *  @brief
    *    Print list of available plugins to log
    */
    void printPlugins() const;


protected:
    /**
    *  @brief
    *    Load plugin library and add all contained plugins
    *
    *  @param[in] filePath
    *    Path to library
    *  @param[in] reload
    *    Reload plugin libraries that are already loaded?
    *
    *  @return
    *    'true' if library has been loaded successfully, else 'false'
    */
    bool loadLibrary(const std::string & filePath, bool reload);

    /**
    *  @brief
    *    Unload plugin library
    *
    *  @param[in] library
    *    Plugin library
    *
    *  @remarks
    *    Before calling the function, it must be made sure that the plugin
    *    library is not used anymore, i.e., no instances of the contained
    *    plugins must be remaining.
    */
    void unloadLibrary(PluginLibrary * library);


protected:
    std::vector<std::string>               m_paths;               ///< Search paths for plugins
    std::vector<Plugin *>                  m_plugins;             ///< Loaded plugins
    std::map<std::string, PluginLibrary *> m_librariesByFilePath; ///< Loaded plugins by path
    std::map<std::string, Plugin *>        m_pluginsByName;       ///< Loaded plugins by name
};


} // namespace gloperate
