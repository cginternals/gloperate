#include <gloperate-glraw/TextureFilter.h>

#include <globjects/logging.h>

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

bool TextureFilter::addFilter(std::initializer_list<std::pair<std::string, QVariantMap>> list)
{
	for( auto filter : list )
	{
		if(!addFilter(filter.first, filter.second))
		{
			return false;
		}
	}
	return true;
}

bool TextureFilter::addFilter(const std::string& name, const QVariantMap& options)
{
	auto filter = glraw::Filter::CreateByName(name, options);
	if (filter)
	{
		addFilter(filter);
		return true;
	}
	else
	{
		globjects::debug() << "Filter: " << name << " not found!" << std::endl;
		return false;
	}
}

bool TextureFilter::addFilter(glraw::AbstractFilter * filter)
{
	assert(filter);
	m_pipeline->appendFilter(filter);
	return true;
}

std::map<std::string, std::string> TextureFilter::allAvailableFilters()
{
	return glraw::Filter::All();
}

void TextureFilter::resetFilters()
{
	m_pipeline->reset();
}

}
