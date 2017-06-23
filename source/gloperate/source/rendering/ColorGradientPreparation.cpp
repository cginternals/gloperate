
#include <gloperate/rendering/ColorGradientPreparation.h>

#include <cppexpose/reflection/AbstractProperty.h>

#include <glbinding/gl/enum.h>

#include <gloperate/rendering/AbstractColorGradient.h>
#include <gloperate/rendering/ColorGradientList.h>
#include <gloperate/rendering/Image.h>


namespace gloperate
{


ColorGradientPreparation::ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize)
: ColorGradientPreparation(gradients, iconSize, {})
{
}

ColorGradientPreparation::ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize, const std::set<std::string> & whitelist)
: m_gradients(gradients)
, m_iconSize(iconSize)
, m_whitelist(whitelist)
{
}

const ColorGradientList & ColorGradientPreparation::gradients() const
{
    return m_gradients;
}

const std::pair<std::uint32_t, std::uint32_t> & ColorGradientPreparation::iconSize() const
{
    return m_iconSize;
}

std::vector<std::string> ColorGradientPreparation::names() const
{
    std::vector<std::string> result;

    fillNames(result);

    return result;
}

void ColorGradientPreparation::fillNames(std::vector<std::string> & names) const
{
    names.clear();

    for (const auto & pair : m_gradients.gradients())
    {
        if (!m_whitelist.empty() && m_whitelist.count(pair.first) == 0)
        {
            continue;
        }

        names.push_back(pair.first);
    }
}

std::vector<Image> ColorGradientPreparation::pixmaps() const
{
    std::vector<Image> result;

    fillPixmaps(result);

    return result;
}

void ColorGradientPreparation::fillPixmaps(std::vector<Image> & pixmaps) const
{
    pixmaps.clear();

    for (const auto & pair : m_gradients.gradients())
    {
        if (!m_whitelist.empty() && m_whitelist.count(pair.first) == 0)
        {
            continue;
        }

        const AbstractColorGradient * gradient = pair.second;

        Image gradientData(m_iconSize.first, m_iconSize.second, gl::GL_RGB, gl::GL_UNSIGNED_INT);

        for (size_t i = 0; i < m_iconSize.second; ++i)
        {
            gradient->fillPixelData(gradientData.data() + i * m_iconSize.first * sizeof(std::uint32_t), m_iconSize.first);
        }

        pixmaps.push_back(gradientData);
    }
}

void ColorGradientPreparation::configureProperty(cppexpose::AbstractProperty * property) const
{
    property->setOption("choices", cppexpose::Variant::fromVector(names()));
    //property->setOption("pixmapSize", cppexpose::Variant::fromValue(iconSize()));
    property->setOption("pixmaps", cppexpose::Variant::fromVector(pixmaps()));
}


} // namespace gloperate
