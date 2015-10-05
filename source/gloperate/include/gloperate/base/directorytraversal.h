
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


// [TODO] Will be removed and replaced by functionality in iozeug

/**
*  @brief
*    List files in directory
*
*  @param[in] directory
*    Path to directory
*  @param[in] fileExtension
*    File extension ("*" for all files)
*  @param[in] recursive
*    Search recursively in sub directories?
*
*  @remarks
*    For each found file, an instance of globjects::File is created.
*    Use globjects::FileRegistry to access all files.
*
*  @see globjects::File
*  @see globjects::FileRegistry
*/
GLOPERATE_API void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recursive = false);


} // namespace gloperate
