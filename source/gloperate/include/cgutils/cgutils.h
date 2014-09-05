#pragma once

#include <string>

#include <cgutils/cgutils_api.h>

namespace cgutils
{

CGUTILS_API void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recoursive=false);

} // namespace gloutils
