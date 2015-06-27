#include <gloperate/resources/GlrawTextureLoader.h>

#include <reflectionzeug/variant/Variant.h>
 

namespace gloperate
{

/**
*  @brief
*    Constructor
*/
GlrawTextureLoader::GlrawTextureLoader()
: Loader<globjects::Texture>()
{
}

/**
*  @brief
*    Destructor
*/
GlrawTextureLoader::~GlrawTextureLoader()
{
}

bool GlrawTextureLoader::canLoad(const std::string & ext) const
{
    return (ext == ".glraw");
}

std::vector<std::string> GlrawTextureLoader::loadingTypes() const
{
    static std::vector<std::string> fileTypes {
        "Glraw Texture (*.glraw)"
    };

    return fileTypes;
}

std::string GlrawTextureLoader::allLoadingTypes() const
{
    return "*.glraw";
}

globjects::Texture * GlrawTextureLoader::load(const std::string & /*filename*/, const reflectionzeug::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{
    // [TODO] Implement!
    return nullptr;
}

} // namespace gloperate
