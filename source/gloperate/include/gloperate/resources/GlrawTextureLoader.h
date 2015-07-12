#pragma once

#include <map>

#include <globjects/Texture.h>

#include <gloperate/resources/Loader.h>

namespace gloperate
{

/**
*  @brief
*    Loader for glraw textures
*/
class GLOPERATE_API GlrawTextureLoader : public Loader<globjects::Texture> 
{
public:
    GlrawTextureLoader();
    virtual ~GlrawTextureLoader() = default;


    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;


    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual globjects::Texture * load(const std::string & filename, std::function<void(int, int)> progress) const override;

protected:

	static std::map<std::string, int32_t> readProperties(std::ifstream & ifs, uint64_t offset);
	static std::vector<char> readRawData(std::ifstream & ifs, uint64_t rawDataOffset);
	static globjects::Texture * createTexture(const std::map<std::string, int32_t> & meta, const std::vector<char> & image);

};

} // namespace gloperate
