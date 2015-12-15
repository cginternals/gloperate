#include <gloperate/base/ColorGradient.h>

#include <cstring>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

namespace gloperate
{

ColorGradient::ColorGradient(const std::string & name)
: m_name(name)
{
}

ColorGradient::~ColorGradient()
{
}

const std::string & ColorGradient::name() const
{
    return m_name;
}

void ColorGradient::setName(const std::string & name)
{
    m_name = name;
}

std::vector<unsigned char> ColorGradient::pixelData(size_t numPixels) const
{
    std::vector<unsigned char> data(numPixels * sizeof(std::uint32_t));

    fillPixelData(data.data(), numPixels);

    return data;
}

void ColorGradient::fillPixelData(unsigned char * data, size_t numPixels) const
{
    for (size_t i = 0; i < numPixels; ++i)
    {
        const float position = i / float(numPixels-1);
        const reflectionzeug::Color color = colorAt(position);
        const std::uint32_t encodedColor = color.bgra();

        std::memmove(&data[i * sizeof(std::uint32_t)], &encodedColor, sizeof(std::uint32_t));
    }
}

globjects::Texture * ColorGradient::generateTexture(size_t numPixels) const
{
    globjects::Texture * texture = globjects::Texture::createDefault(gl::GL_TEXTURE_1D);

    std::vector<unsigned char> data = pixelData(numPixels);

    texture->image1D(0, gl::GL_RGBA, numPixels, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data.data());

    return texture;
}


StaticGradient::StaticGradient(const std::string & name, const reflectionzeug::Color & color)
: ColorGradient(name)
, m_color(color)
{
}

void StaticGradient::fillPixelData(unsigned char * data, size_t numPixels) const
{
    const std::uint32_t encodedColor = m_color.bgra();

    for (size_t i = 0; i < numPixels; ++i)
    {
        std::memcpy(&data[i * sizeof(std::uint32_t)], &encodedColor, sizeof(std::uint32_t));
    }
}

reflectionzeug::Color StaticGradient::colorAt(float /*position*/) const
{
    return m_color;
}

LinearGradient::LinearGradient(const std::string & name, const reflectionzeug::Color & color1, const reflectionzeug::Color & color2)
: ColorGradient(name)
, m_color1(color1)
, m_color2(color2)
{
}

reflectionzeug::Color LinearGradient::colorAt(float position) const
{
    return m_color1.interpolate(m_color2, position);
}

} // namespace gloperate
