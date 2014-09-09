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
        *    Get default path to search for plugins
        *
        *  @return
        *    Path to search for plugin libraries
        *
        *  @remarks
        *    Usually, this will return the directory of the current executable
        */
        std::string defaultPluginDirectory() const;

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
        *    Load plugin
        *
        *  @param[in] name
        *    Name of the plugin library (only the base name, do not add 'lib' or '.so'/'.dll', those are added automatically
        *  @param[in] path
        *    Path at which to search for plugin libraries
        */
        void loadPlugin(const std::string & name, const std::string & path = "");

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
        std::vector<PluginLibrary *>    m_libraries;     /**< List of libraries */
        std::vector<Plugin *>           m_plugins;       /**< List of plugins */
        std::map<std::string, Plugin *> m_pluginsByName; /**< Map of name -> plugin */


};


} // namespace gloperate
