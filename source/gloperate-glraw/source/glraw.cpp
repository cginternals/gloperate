#include <gloperate-glraw/TextureFilter.h>

// !!!ATTENTION!!!
// This header also includes GL.h indirectly.
// Do not use anything from gl namespace.
#include <glraw/ConvertManager.h>
#include <glraw/Converter.h>

namespace gloperate_glraw
{

TextureFilter::TextureFilter()
: m_converter(new glraw::ConvertManager())
{
	m_converter->setConverter(new glraw::Converter());
}

TextureFilter::~TextureFilter()
{
	delete m_converter;
}

void TextureFilter::transfer(glraw::AssetInformation & info)
{
	m_converter;
}

std::vector<std::string> TextureFilter::allAvailableFilters() const
{
	return std::vector<std::string>();
}

}