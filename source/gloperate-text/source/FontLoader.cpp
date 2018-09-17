
#include <gloperate-text/FontLoader.h>

#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <set>
#include <map>

#include <cppassist/logging/logging.h>
#include <cppassist/string/conversion.h>
#include <cppassist/string/manipulation.h>
#include <cppassist/fs/RawFile.h>

#include <cppfs/FilePath.h>

#include <glbinding/gl/enum.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <openll/FontFace.h>
#include <openll/FontLoader.h>


namespace gloperate_text
{


CPPEXPOSE_COMPONENT(FontLoader, gloperate::AbstractLoader)


FontLoader::FontLoader(gloperate::Environment * environment)
: gloperate::Loader<openll::FontFace>(environment)
{
}

bool FontLoader::canLoad(const std::string & ext) const
{
    return ext == "fnt";
}

std::vector<std::string> FontLoader::loadingTypes() const
{
    return { "Bitmap Font (*.fnt)" };
}

std::string FontLoader::allLoadingTypes() const
{
    return "*.fnt";
}

openll::FontFace * FontLoader::load(const std::string & filename
    , const cppexpose::Variant &, const std::function<void(int, int)>) const
{
    return openll::FontLoader::load(filename).release();
}


} // namespace gloperate_text
