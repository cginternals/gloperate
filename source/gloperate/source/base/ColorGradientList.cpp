#include <gloperate/base/ColorGradientList.h>

#include <algorithm>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/base/ColorGradient.h>

namespace gloperate
{

ColorGradientList::ColorGradientList()
{
}

ColorGradientList::ColorGradientList(std::initializer_list<ColorGradient*> gradients)
{
    for (auto gradient : gradients)
    {
        add(gradient);
    }
}

ColorGradientList::~ColorGradientList()
{
    for (const auto & pair : m_gradients)
    {
        delete pair.second;
    }
}

size_t ColorGradientList::size() const
{
    return m_gradients.size();
}

const std::map<std::string, const ColorGradient *> & ColorGradientList::gradients() const
{
    return reinterpret_cast<const std::map<std::string, const ColorGradient *> &>(m_gradients);
}

void ColorGradientList::add(ColorGradient * gradient)
{
    m_gradients.emplace(gradient->name(), gradient);
}

const ColorGradient * ColorGradientList::at(const std::string & name) const
{
    auto iterator = m_gradients.find(name);

    if (iterator == m_gradients.end())
    {
        return nullptr;
    }

    return iterator->second;
}

ColorGradient * ColorGradientList::at(const std::string & name)
{
    auto iterator = m_gradients.find(name);

    if (iterator == m_gradients.end())
    {
        return nullptr;
    }

    return iterator->second;
}

size_t ColorGradientList::indexOf(const std::string & name)
{
    auto iterator = m_gradients.find(name);

    return std::distance(m_gradients.begin(), iterator);
}

std::vector<unsigned char> ColorGradientList::pixelData(size_t numPixels) const
{
    std::vector<unsigned char> data(numPixels * m_gradients.size() * sizeof(std::uint32_t));

    size_t i = 0;
    for (const auto & pair : m_gradients)
    {
        const ColorGradient * gradient = pair.second;

        gradient->fillPixelData(&data[i * numPixels * sizeof(std::uint32_t)], numPixels);

        ++i;
    }

    return data;
}

globjects::Texture * ColorGradientList::generateTexture(size_t numPixels) const
{
    globjects::Texture * texture = globjects::Texture::createDefault();

    std::vector<unsigned char> data = pixelData(numPixels);

    texture->image2D(0, gl::GL_RGBA, numPixels, m_gradients.size(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data.data());

    return texture;
}

} // namespace gloperate
