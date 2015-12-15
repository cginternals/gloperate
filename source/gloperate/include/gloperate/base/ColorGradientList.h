#pragma once

#include <map>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/AbstractColorGradient.h>

namespace globjects
{

class Texture;

}


namespace gloperate
{

class GLOPERATE_API ColorGradientList
{
public:
    ColorGradientList();
    ColorGradientList(std::initializer_list<AbstractColorGradient*> gradients);

    template <typename... Args>
    ColorGradientList(AbstractColorGradient * gradient, Args... args);

    virtual ~ColorGradientList();

    size_t size() const;

    const std::map<std::string, const AbstractColorGradient *> & gradients() const;

    void add(AbstractColorGradient * gradient);
    const AbstractColorGradient * at(const std::string & name) const;
    AbstractColorGradient * at(const std::string & name);

    size_t indexOf(const std::string & name) const;

    std::vector<unsigned char> pixelData(size_t numPixels) const;

    globjects::Texture * generateTexture(size_t numPixels) const;

protected:
    std::map<std::string, AbstractColorGradient *> m_gradients;
};

} // namespace gloperate

#include <gloperate/base/ColorGradientList.hpp>
