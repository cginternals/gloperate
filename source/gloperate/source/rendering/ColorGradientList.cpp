
#include <gloperate/rendering/ColorGradientList.h>

#include <algorithm>

#include <glm/glm.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/AbstractColorGradient.h>
#include <gloperate/rendering/Image.h>


namespace gloperate
{


std::unique_ptr<globjects::Texture> ColorGradientList::generateTexture(const std::vector<ColorGradientList *> & colorGradientLists, size_t numPixels)
{
    auto texture = globjects::Texture::createDefault();

    size_t numberOfGradients = 0;
    for (auto list : colorGradientLists)
    {
        numberOfGradients += list->size();
    }
    size_t gradientSize = numPixels * sizeof(std::uint32_t);

    std::vector<unsigned char> data(numberOfGradients * gradientSize);
    size_t offset = 0;
    for (auto list : colorGradientLists)
    {
        list->appendPixelData(numPixels, &(data[offset * gradientSize]));
        offset += list->size();
    }

    texture->image2D(0, gl::GL_RGBA, numPixels, numberOfGradients, 0, gl::GL_BGRA, gl::GL_UNSIGNED_BYTE, data.data());

    return texture;
}

ColorGradientList::ColorGradientList()
{
}

ColorGradientList::ColorGradientList(std::initializer_list<AbstractColorGradient*> gradients)
{
    for (auto gradient : gradients)
    {
        add(std::unique_ptr<AbstractColorGradient>(gradient));
    }
}

ColorGradientList::~ColorGradientList()
{
}

size_t ColorGradientList::size() const
{
    return m_gradients.size();
}

const std::map<std::string, const AbstractColorGradient *> & ColorGradientList::gradients() const
{
    return reinterpret_cast<const std::map<std::string, const AbstractColorGradient *> &>(m_gradients);
}

std::vector<std::string> ColorGradientList::names() const
{
    std::vector<std::string> names;

    for (const auto & pair : m_gradients)
    {
        names.push_back(pair.first);
    }

    return names;
}

std::vector<Image> ColorGradientList::pixmaps(const glm::uvec2 & size) const
{
    std::vector<Image> pixmaps;

    for (const auto & pair : m_gradients)
    {
        const AbstractColorGradient * gradient = pair.second.get();

        Image gradientData(size.x, size.y, gl::GL_RGB, gl::GL_UNSIGNED_INT);

        for (size_t i = 0; i < size.y; ++i)
        {
            gradient->fillPixelData(gradientData.data() + i * size.x * sizeof(std::uint32_t), size.x);
        }

        pixmaps.push_back(gradientData);
    }

    return pixmaps;
}

void ColorGradientList::add(std::unique_ptr<AbstractColorGradient> && gradient)
{
    const auto it = m_gradients.find(gradient->name());

    if (it == m_gradients.end())
    {
        m_gradients.emplace(gradient->name(), std::move(gradient));
    }
    else
    {
        it->second = std::move(gradient);
    }
}

const AbstractColorGradient * ColorGradientList::at(const std::string & name) const
{
    auto iterator = m_gradients.find(name);

    if (iterator == m_gradients.end())
    {
        return nullptr;
    }

    return iterator->second.get();
}

AbstractColorGradient * ColorGradientList::at(const std::string & name)
{
    auto iterator = m_gradients.find(name);

    if (iterator == m_gradients.end())
    {
        return nullptr;
    }

    return iterator->second.get();
}

int ColorGradientList::indexOf(const std::string & name) const
{
    auto it = m_gradients.find(name);

    if (it == m_gradients.end())
    {
        return -1;
    }

    return static_cast<int>(std::distance(m_gradients.begin(), it));
}

void ColorGradientList::appendPixelData(size_t numPixels, unsigned char * start) const
{
    size_t i = 0;
    for (const auto & pair : m_gradients)
    {
        const auto & gradient = pair.second;

        gradient->fillPixelData(start + (i * numPixels * sizeof(std::uint32_t)), numPixels);

        ++i;
    }
}

std::unique_ptr<globjects::Texture> ColorGradientList::generateTexture(size_t numPixels) const
{
    auto texture = globjects::Texture::createDefault();

    std::vector<unsigned char> data(numPixels * m_gradients.size() * sizeof(std::uint32_t));
    appendPixelData(numPixels, &(data[0]));

    texture->image2D(0, gl::GL_RGBA, numPixels, m_gradients.size(), 0, gl::GL_BGRA, gl::GL_UNSIGNED_BYTE, data.data());

    return texture;
}


} // namespace gloperate
