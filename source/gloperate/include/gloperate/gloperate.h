
#pragma once

#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Get path to gloperate data files
*
*    Uses cpplocate to locate the module 'gloperate' and reads the
*    data path from it's configuration.
*
*  @return
*    Path to gloperate data files, or '' if module was not found
*/
GLOPERATE_API const std::string & dataPath();

/**
*  @brief
*    Get path to gloperate plugins
*
*    Uses cpplocate to locate the module 'gloperate' and reads the
*    plugins path from it's configuration.
*
*  @return
*    Path to gloperate plugins, or '' if module was not found
*/
GLOPERATE_API const std::string & pluginPath();


} // namespace gloperate
