/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/plugin/PluginLibrary.h>
#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
PluginLibrary::PluginLibrary(const std::string & filename)
: m_filename(filename)
, m_initPluginPtr(nullptr)
, m_getNumOfPluginsPtr(nullptr)
, m_getPluginPtr(nullptr)
, m_deinitPluginPtr(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
PluginLibrary::~PluginLibrary()
{
}

/**
*  @brief
*    Get filename
*/
const std::string & PluginLibrary::filename() const
{
    return m_filename;
}

/**
*  @brief
*    Check if plugin library is valid
*/
bool PluginLibrary::isValid() const
{
    return (m_initPluginPtr      != nullptr &&
            m_getNumOfPluginsPtr != nullptr &&
            m_getPluginPtr       != nullptr &&
            m_deinitPluginPtr    != nullptr );
}

/**
*  @brief
*    Initialize plugin library
*/
void PluginLibrary::initialize()
{
    // Call plugin function
    if (m_initPluginPtr != nullptr) {
        (*m_initPluginPtr)();
    }
}

/**
*  @brief
*    Get number of plugins contained in the library
*/
unsigned int PluginLibrary::getNumOfPlugins() const
{
    // Call plugin function
    if (m_getNumOfPluginsPtr != nullptr) {
        return (*m_getNumOfPluginsPtr)();
    } else {
        return 0;
    }
}

/**
*  @brief
*    Get plugin contained in the library
*/
gloperate::Plugin * PluginLibrary::getPlugin(unsigned int index) const
{
    // Call plugin function
    if (m_getPluginPtr != nullptr) {
        return (*m_getPluginPtr)(index);
    } else {
        return nullptr;
    }
}

/**
*  @brief
*    Deinitialize plugin library
*/
void PluginLibrary::deinitialize()
{
    // Call plugin function
    if (m_deinitPluginPtr != nullptr) {
        (*m_deinitPluginPtr)();
    }
}


} // namespace gloperate
