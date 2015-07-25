
#include <gloperate-glraw/TextureFilter.h>

#include <QByteArray>

#include <glbinding/ContextHandle.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>
#include <globjects/Texture.h>

#include <gloperate-qt/QtOpenGLWindow.h>

namespace gloperate_glraw
{

globjects::Texture * TextureFilter::process(const globjects::Texture * const input)
{
	assert(input);
	assert(m_pipeline);

	glraw::AssetInformation info = generateAssetInformation(input);

	
	auto x = input->getImage(0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE);
	m_rawData = QByteArray::fromRawData(reinterpret_cast<char*>(x.data()), x.size());
	

	m_canvas->doneCurrent();
	transfer(info);
	m_canvas->makeCurrent();


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
	const int width = input->getLevelParameter(0, gl::GL_TEXTURE_WIDTH);
	const int height = input->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT);

	glraw::AssetInformation info;
	info.setProperty("width", width);
	info.setProperty("height", height);

	return info;
}

}