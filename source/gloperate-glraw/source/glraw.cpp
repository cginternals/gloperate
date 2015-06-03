#include <gloperate-glraw/TextureFilter.h>

#include <globjects/logging.h>

// !!!ATTENTION!!!
// This header also includes GL.h indirectly.
// Do not use anything from gl namespace.
#include <glraw/MemoryProcessor.h>
#include <glraw/Converter.h>
#include <glraw/filter/Filter.hpp>

namespace gloperate_glraw
{

TextureFilter::TextureFilter(std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas)
	: m_pipeline(new glraw::MemoryProcessor())
	, m_canvas(canvas)
{
	m_pipeline->setConverter(new glraw::Converter());
}

TextureFilter::~TextureFilter()
{
	delete m_pipeline;
}

void TextureFilter::transfer(glraw::AssetInformation & info)
{
	m_pipeline->process(m_rawData, info);
}

bool TextureFilter::addFilter(const std::string& name, const QVariantMap& options)
{
	try
	{
		auto filter = glraw::Filter::CreateByName(name, options);
		if (filter)
		{
			m_pipeline->appendFilter(filter);
			return true;
		}
		else
		{
			globjects::debug() << "Filter: " << name << " not found!" << std::endl;
			return false;
		}
	}
	catch (std::exception& e)
	{
		globjects::debug() << "ERROR @" << name << ": " << e.what() << std::endl;
		return false;
	}
}

std::vector<std::string> TextureFilter::allAvailableFilters() const
{
	return std::vector<std::string>();
}

}
