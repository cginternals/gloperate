#include <gloperate/tools/ColorGradientPreparation.h>

#include <reflectionzeug/property/AbstractProperty.h>

#include <gloperate/base/AbstractColorGradient.h>
#include <gloperate/base/ColorGradientList.h>


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

std::vector<std::vector<unsigned char>> ColorGradientPreparation::pixmaps() const
{
    std::vector<std::vector<unsigned char>> result;

    fillPixmaps(result);

    return result;
}

void ColorGradientPreparation::fillPixmaps(std::vector<std::vector<unsigned char>> & pixmaps) const
{
    pixmaps.clear();

    for (const auto & pair : m_gradients.gradients())
    {
        if (!m_whitelist.empty() && m_whitelist.count(pair.first) == 0)
        {
            continue;
        }

        const gloperate::AbstractColorGradient * gradient = pair.second;

        std::vector<unsigned char> gradientData(m_iconSize.first * m_iconSize.second * sizeof(std::uint32_t));

        for (size_t i = 0; i < m_iconSize.second; ++i)
        {
            gradient->fillPixelData(gradientData.data()+i*m_iconSize.first*sizeof(std::uint32_t), m_iconSize.first);
        }

        pixmaps.push_back(gradientData);
    }
}

void ColorGradientPreparation::configureProperty(reflectionzeug::AbstractProperty * property) const
{
    property->setOption("pixmapSize", reflectionzeug::Variant::fromValue(iconSize()));
    property->setOption("choices", reflectionzeug::Variant::fromValue(names()));
    property->setOption("pixmaps", reflectionzeug::Variant::fromValue(pixmaps()));
}


} // namespace gloperate
