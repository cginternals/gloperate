#pragma once

#include <map>

namespace globjects
{

class Texture;

}

#include <gloperate/gloperate_api.h>

#include <gloperate/base/ColorGradient.h>

namespace gloperate
{

class GLOPERATE_API ColorGradientList
{
public:
    ColorGradientList();
    ColorGradientList(std::initializer_list<ColorGradient*> gradients);

    template <typename... Args>
    ColorGradientList(ColorGradient * gradient, Args... args);

    virtual ~ColorGradientList();

    size_t size() const;

    const std::map<std::string, const ColorGradient *> & gradients() const;

    void add(ColorGradient * gradient);
    const ColorGradient * at(const std::string & name) const;
    ColorGradient * at(const std::string & name);
    size_t indexOf(const std::string & name);

    std::vector<unsigned char> pixelData(size_t numPixels) const;

    globjects::Texture * generateTexture(size_t numPixels) const;

protected:
    std::map<std::string, ColorGradient *> m_gradients;
};

} // namespace gloperate

#include <gloperate/base/ColorGradientList.hpp>
