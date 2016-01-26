#include <gloperate/base/AbstractColorGradient.h>

#include <cstring>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>


namespace gloperate
{


AbstractColorGradient::AbstractColorGradient(const std::string & name)
: m_name(name)
{
}

AbstractColorGradient::~AbstractColorGradient()
{
}

const std::string & AbstractColorGradient::name() const
{
    return m_name;
}

void AbstractColorGradient::setName(const std::string & name)
{
    m_name = name;
}

std::vector<unsigned char> AbstractColorGradient::pixelData(size_t numPixels) const
{
    std::vector<unsigned char> data(numPixels * sizeof(std::uint32_t));

    fillPixelData(data.data(), numPixels);

    return data;
}

void AbstractColorGradient::fillPixelData(unsigned char * data, size_t numPixels) const
{
    for (size_t i = 0; i < numPixels; ++i)
    {
        const float position = i / float(numPixels-1);
        const reflectionzeug::Color color = colorAt(position);
        const std::uint32_t encodedColor = color.bgra();

        std::memmove(&data[i * sizeof(std::uint32_t)], &encodedColor, sizeof(std::uint32_t));
    }
}

globjects::Texture * AbstractColorGradient::generateTexture(size_t numPixels) const
{
    globjects::Texture * texture = globjects::Texture::createDefault(gl::GL_TEXTURE_1D);

    std::vector<unsigned char> data = pixelData(numPixels);

    texture->image1D(0, gl::GL_RGBA, numPixels, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data.data());

    return texture;
}


} // namespace gloperate
