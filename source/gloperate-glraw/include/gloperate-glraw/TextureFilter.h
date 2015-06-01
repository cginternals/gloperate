#include <gloperate-glraw/gloperate-glraw_api.h>

#include <QVariantMap>

#include <glraw/AssetInformation.h>

namespace globjects
{
	class Texture;
}

namespace glraw
{
	class MemoryProcessor;
	class AssetInformation;
}

namespace gloperate_glraw
{

class GLOPERATE_GLRAW_API TextureFilter
{
public:
	TextureFilter();
	~TextureFilter();

	bool addFilter(const std::string& name, const QVariantMap& options);
	void resetFilters();

	std::vector<std::string> allAvailableFilters() const;

	globjects::Texture * process(const globjects::Texture * const input);

private:

	void transfer(glraw::AssetInformation & info);
	glraw::AssetInformation generateAssetInformation(const globjects::Texture * const);

	glraw::MemoryProcessor * m_pipeline;
	QByteArray m_rawData;
};

}