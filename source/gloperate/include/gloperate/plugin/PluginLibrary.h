
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
using init_ptr = void (*)();

/**
*  @brief
*    Function to get the number of plugins contained in a library
*/
using numPlugins_ptr = int (*)();

/**
*  @brief
*    Function to get a specific plugin of a plugin library
*/
using plugin_ptr = gloperate::Plugin * (*)(unsigned int);

/**
*  @brief
*    Function to deinitialize a plugin library
*/
using deinit_ptr = void (*)();


/**
*  @brief
*    Represents a dynamic library that contains plugins
*/
class GLOPERATE_API PluginLibrary 
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] filePath
    *    Path to the loaded library
    */
    PluginLibrary(const std::string & filePath);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PluginLibrary();

    /**
    *  @brief
    *    Get path to library file
    *
    *  @return
    *    Path to the loaded library
    */
    const std::string & filePath() const;

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
    *    De-initialize plugin library
    */
    void deinitialize();

    /**
    *  @brief
    *    Get number of plugins contained in the library
    *
    *  @return
    *    Number of plugins
    */
    unsigned int numPlugins() const;

    /**
    *  @brief
    *    Get plugin by index
    *
    *  @param[in] index
    *    Plugin number
    *
    *  @return
    *    Pointer to the plugin, nullptr on error
    */
    gloperate::Plugin * plugin(unsigned int index) const;                                                                                                                                        


protected:
    std::string     m_filePath;
    init_ptr        m_initPtr;
    deinit_ptr      m_deinitPtr;
    numPlugins_ptr  m_numPluginsPtr;
    plugin_ptr      m_pluginPtr;
};


} // namespace gloperate
