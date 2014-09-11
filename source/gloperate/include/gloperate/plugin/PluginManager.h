/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <string>
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
class GLOPERATE_API PluginManager {


    public:
        /**
        *  @brief
        *    Initialize plugin manager
        *
        *  @param[in] executablePath
        *    Path to the current executable
        *
        *  @remarks
        *    This function has to be called to tell the plugin manager the path
        *    to the current executable, so it can determine the default search directory
        *    for plugins. On Window, path is ignored, since the executable path
        *    can be obtained automatically.
        */
        static void init(const std::string & executablePath = "");


    protected:
        /** Default path to look for plugins, by default the path of the executable */
        static std::string s_defaultPluginPath;


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
        *    Get plugin directory
        *
        *  @return
        *    Directory from which plugins are loaded
        */
        std::string pluginDirectory() const;

        /**
        *  @brief
        *    Set plugin directory
        *
        *  @param[in] path
        *    Directory from which plugins are loaded
        *
        *  @remarks
        *    If the plugin directory is not set, the default search path is used
        */
        void setPluginDirectory(const std::string & path);

        /**
        *  @brief
        *    Scan for plugins and load all found plugins
        *
        *  @param[in] identifier
        *    If set, only libraries that contain the specified substring are loaded
        */
        void scanPlugins(const std::string & identifier = "");

        /**
        *  @brief
        *    Load plugin
        *
        *  @param[in] name
        *    Name of the plugin library (only the base name, do not add 'lib' or '.so'/'.dll', those are added automatically
        *  @param[in] path
        *    Path at which to search for plugin libraries. If "", the default search path is used
        */
        void loadPlugin(const std::string & name);

        /**
        *  @brief
        *    Load plugin library
        *
        *  @param[in] filename
        *    Path to dynamic library
        */
        void loadPluginLibrary(const std::string & filename);

        /**
        *  @brief
        *    Get available plugins
        *
        *  @return
        *    List of loaded plugins
        */
        const std::vector<Plugin *> & plugins() const;

        /**
        *  @brief
        *    Get plugin by name
        *
        *  @param[in] name
        *    Name of plugin
        *
        *  @return
        *    Pointer to plugin, nullptr on error
        */
        Plugin * plugin(const std::string & name) const;


    protected:
        std::string                     m_pluginDirectory;  /**< Directory from which plugins are loaded */
        std::vector<PluginLibrary *>    m_libraries;        /**< List of libraries */
        std::vector<Plugin *>           m_plugins;          /**< List of plugins */
        std::map<std::string, Plugin *> m_pluginsByName;    /**< Map of name -> plugin */


};


} // namespace gloperate
