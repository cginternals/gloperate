#include <gloperate/resources/GlrawTextureLoader.h>
 

namespace gloperate
{


GlrawTextureLoader::GlrawTextureLoader()
: Loader<globjects::Texture>()
{
}

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

globjects::Texture * GlrawTextureLoader::load(const std::string & /*filename*/) const
{
	// [TODO] Implement!
    return nullptr;
}


} // namespace gloperate
