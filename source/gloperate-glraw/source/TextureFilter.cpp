
#include <gloperate-glraw/TextureFilter.h>

#include <QByteArray>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

namespace gloperate_glraw
{

globjects::Texture * TextureFilter::process(const globjects::Texture * const input)
{
	assert(input);
	assert(m_converter);

	glraw::AssetInformation info = generateAssetInformation(input);
	input->getImage(0, gl::GL_RGBA8, gl::GL_UNSIGNED_BYTE, m_rawData.data());

	transfer(info);

	if (m_rawData.isNull())
		return nullptr;

	const int width = info.property("width").toInt();
	const int height = info.property("height").toInt();

	globjects::Texture * output = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
	output->image2D(0, gl::GL_RGBA8, width, height, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, m_rawData.data());

	return output;
}

glraw::AssetInformation TextureFilter::generateAssetInformation(const globjects::Texture * const input)
{
	glraw::AssetInformation info;
	const int width = input->getLevelParameter(0, gl::GL_TEXTURE_WIDTH);
	const int height = input->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT);
	const int depth = input->getLevelParameter(0, gl::GL_TEXTURE_DEPTH);

	m_rawData.resize(width * height * depth);

	info.setProperty("width", width);
	info.setProperty("height", height);
	return info;
}

}