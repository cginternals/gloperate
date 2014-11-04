#pragma once

#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

class Plugin;

/** \brief Function to initialize a plugin library
*/
using init_ptr = void (*)();

/** \brief Function to get the number of plugins contained in a library
*/
using numPlugins_ptr = int (*)();

/** \brief Function to get a specific plugin of a plugin library
*/
using plugin_ptr = gloperate::Plugin * (*)(unsigned int);

/** \brief Function to deinitialize a plugin library
*/
using deinit_ptr = void (*)();


/** \brief Describes a dynamic library that contains plugins
*/
class GLOPERATE_API PluginLibrary 
{
public:
    PluginLibrary(const std::string & fileName);
    virtual ~PluginLibrary();

    const std::string & fileName() const;

    /** \return 'true' if the plugin has been loaded successfully, else 'false'
    */
    bool isValid() const;

    void initialize();
    void deinitialize();

    unsigned int numPlugins() const;

    gloperate::Plugin * plugin(unsigned int index) const;                                                                                                                                        

protected:
    std::string     m_fileName;

    init_ptr        m_initPtr;
    deinit_ptr      m_deinitPtr;
    numPlugins_ptr  m_numPluginsPtr;
    plugin_ptr      m_pluginPtr;
};

} // namespace gloperate
