/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <string>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Plugin;


/**
*  @brief
*    Function to initialize a plugin library
*/
typedef void (*INITPLUGIN_PTR)();

/**
*  @brief
*    Function to get the number of plugins contained in a library
*/
typedef int (*GETNUMOFPLUGINS_PTR)();

/**
*  @brief
*    Function to get a specific plugin of a plugin library
*/
typedef gloperate::Plugin * (*GETPLUGIN_PTR)(unsigned int);

/**
*  @brief
*    Function to deinitialize a plugin library
*/
typedef void (*DEINITPLUGIN_PTR)();


/**
*  @brief
*    Describes a dynamic library that contains plugins
*/
class GLOPERATE_API PluginLibrary {


    public:
        /**
        *  @brief
        *    Constructor
        *
        *  @param[in] filename
        *    Filename
        */
        PluginLibrary(const std::string & filename);

        /**
        *  @brief
        *    Destructor
        */
        virtual ~PluginLibrary();

        /**
        *  @brief
        *    Get filename
        *
        *  @return
        *    Filename
        */
        const std::string & filename() const;

        /**
        *  @brief
        *    Check if plugin library is valid
        *
        *  @return
        *    'true' if the plugin has been loaded successfully, else 'false'
        */
        bool isValid() const;

        /**
        *  @brief
        *    Initialize plugin library
        */
        void initialize();

        /**
        *  @brief
        *    Get number of plugins contained in the library
        *
        *  @return
        *    Number of plugins
        */
        unsigned int getNumOfPlugins() const;

        /**
        *  @brief
        *    Get plugin contained in the library
        *
        *  @param[in] index
        *    Index of the plugin
        *
        *  @return
        *    Plugin, nullptr on error
        */
        gloperate::Plugin * getPlugin(unsigned int index) const;

        /**
        *  @brief
        *    Deinitialize plugin library
        */
        void deinitialize();


    protected:
        std::string         m_filename;             /**< Filename of the plugin library */
        INITPLUGIN_PTR      m_initPluginPtr;        /**< Function to initialize a plugin library */
        GETNUMOFPLUGINS_PTR m_getNumOfPluginsPtr;   /**< Function to get the number of plugins contained in a library */
        GETPLUGIN_PTR       m_getPluginPtr;         /**< Function to get a specific plugin of a plugin library */
        DEINITPLUGIN_PTR    m_deinitPluginPtr;      /**< Function to deinitialize a plugin library */


};


} // namespace gloperate
