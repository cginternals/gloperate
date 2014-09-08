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
        std::vector<PluginLibrary *>    m_libraries;     /**< List of libraries */
        std::vector<Plugin *>           m_plugins;       /**< List of plugins */
        std::map<std::string, Plugin *> m_pluginsByName; /**< Map of name -> plugin */


};


} // namespace gloperate
