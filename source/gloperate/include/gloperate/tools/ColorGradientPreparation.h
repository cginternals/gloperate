#pragma once

#include <string>
#include <vector>
#include <utility>

#include <gloperate/gloperate_api.h>


namespace reflectionzeug
{


class AbstractProperty;


} // namespace reflectionzeug


namespace gloperate
{


class ColorGradientList;


class GLOPERATE_API ColorGradientPreparation
{
public:
    ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize);

    const ColorGradientList & gradients() const;
    const std::pair<std::uint32_t, std::uint32_t> & iconSize() const;
    std::vector<std::string> names() const;
    std::vector<std::vector<unsigned char>> pixmaps() const;

    void fillNames(std::vector<std::string> & names) const;
    void fillPixmaps(std::vector<std::vector<unsigned char>> & pixmaps) const;

    void configureProperty(reflectionzeug::AbstractProperty * property) const;
protected:
    const ColorGradientList & m_gradients;
    std::pair<std::uint32_t, std::uint32_t> m_iconSize;
};


} // namespace gloperate
