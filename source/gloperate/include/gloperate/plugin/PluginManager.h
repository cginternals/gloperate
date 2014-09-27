#pragma once

#include <string>
#include <vector>
#include <map>
#include <gloperate/gloperate_api.h>


namespace gloperate
{

class PluginLibrary;
class Plugin;

/** \brief Class for loading and managing plugins
*/
class GLOPERATE_API PluginManager 
{
public:
    /** \brief Initialize plugin manager
        \param[in] executablePath Path to the current executable

        \remarks This function has to be called to tell the plugin manager the path
         to the current executable, so it can determine the default search directory
         for plugins. On Window, path is ignored, since the executable path
         can be obtained automatically.
    */
    static void init(const std::string & executablePath = "");


protected:
    /** Default path to look for plugins, by default the path of the executable 
    */
    static std::string s_defaultScanPath;

public:
    PluginManager();
    virtual ~PluginManager();

    /** \brief Set scan directory
        \param[in] path Directory from which plugins are loaded

        \remarks If the plugin directory is not set, the default search path is used
    */
    void setScanDirectory(const std::string & path);
    std::string scanDirectory() const;

    /** \brief scan for plugins and load all found ones
        \@param[in] identifier If set, only libraries that contain the specified substring are loaded
    */
    void scan(const std::string & identifier = "");

    /** \param[in] name Name of the plugin library (only the base name,
         do not add 'lib' or '.so'/'.dll', those are added automatically
        \param[in] path Path at which to search for plugin libraries. If "", the default search path is used
    */
    void load(const std::string & name);

    const std::vector<Plugin *> & plugins() const;
    Plugin * plugin(const std::string & name) const;

    /** \brief Print list of available plugins to log
    */
    void printPlugins() const;

protected:
    void loadLibrary(const std::string & filename);

protected:
    std::string                     m_scanPath;
    std::vector<PluginLibrary *>    m_libraries;
    std::vector<Plugin *>           m_plugins;
    std::map<std::string, Plugin *> m_pluginsByName;
};

} // namespace gloperate
