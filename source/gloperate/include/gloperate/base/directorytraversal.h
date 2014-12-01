#pragma once

#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

GLOPERATE_API void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recoursive=false);

} // namespace globjectsutils
