
#pragma once


#include <string>
#include <set>
#include <vector>
#include <map>

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
    /**
    *  @brief
    *    Initialize plugin manager
    *
    *  @param[in] applicationPath
    *    Path to directory containing the application executable
    *
    *  @remarks
    *    This function has to be called to tell the plugin
    *    manager the path to the current executable, so it can
    *    determine the default search directory for plugins.
    */
    static void init(const std::string & applicationPath = "");


public:
    PluginManager();
    virtual ~PluginManager();

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
    void addPath(const std::string & path);

    /**
    *  @brief
    *    Remove plugin search path
    *
    *  @param[in] path
    *    Plugin search path
    */
    void removePath(const std::string & path);

    void setPaths(const std::vector<std::string> & paths);
    const std::vector<std::string> & paths() const;

    /**
    *  @brief
    *    Scan for plugins and load all found ones
    *
    *  @param[in] identifier
    *    If set, only libraries with a name containing
    *    the specified substring are considered.
    */
    void scan(const std::string & identifier = "", bool reload = false);

    /**
    *  @brief
    *    Load plugin library
    *
    *  @param[in] filePath
    *    Filename of the plugin library
    */
    bool load(const std::string & filePath, bool reload = true);

    const std::vector<Plugin *> & plugins() const;
    Plugin * plugin(const std::string & name) const;

    /**
    *  @brief
    *    Print list of available plugins to log
    */
    void printPlugins() const;


protected:
    bool loadLibrary(const std::string & filePath, bool reload);
    void unloadLibrary(PluginLibrary * library) const;


protected:
    static std::string s_applicationPath;   ///< Default path to look for plugins, by default the path of the executable


protected:
    std::vector<std::string> m_paths;

    std::map<std::string, PluginLibrary *> m_librariesByFilePath;

    std::vector<Plugin *> m_plugins;
    std::map<std::string, Plugin *> m_pluginsByName;
};


} // namespace gloperate
