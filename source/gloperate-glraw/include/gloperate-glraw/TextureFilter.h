#include <gloperate-glraw/gloperate-glraw_api.h>

#include <initializer_list>
#include <memory>

#include <QVariantMap>

#include <glraw/AssetInformation.h>

namespace globjects
{
	class Texture;
}

namespace gloperate_qt
{
	class QtOpenGLWindow;
}

namespace glraw
{
	class MemoryProcessor;
	class AssetInformation;
	class AbstractFilter;
}

namespace gloperate_glraw
{

class GLOPERATE_GLRAW_API TextureFilter
{
public:
	TextureFilter(std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas);
	~TextureFilter();

	bool addFilter(std::initializer_list<std::pair<std::string, QVariantMap>> list);
	bool addFilter(const std::string& name, const QVariantMap& options = QVariantMap());
	bool addFilter(glraw::AbstractFilter * filter);
	void resetFilters();

	std::vector<std::string> allAvailableFilters() const;

	globjects::Texture * process(const globjects::Texture * const input);

private:

	void transfer(glraw::AssetInformation & info);
	glraw::AssetInformation generateAssetInformation(const globjects::Texture * const);

	glraw::MemoryProcessor * m_pipeline;
	QByteArray m_rawData;

	std::unique_ptr<gloperate_qt::QtOpenGLWindow> & m_canvas;
};

}
