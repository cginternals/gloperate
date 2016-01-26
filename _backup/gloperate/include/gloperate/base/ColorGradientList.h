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

/**
 * @brief The ColorGradientList represents and manages a list of AbstractColorGradients.
 *
 * The color gradients are stored and sorted by their names, requiring a lookup by name.
 *
 * A texture containing all gradients can be created. For a lookup in this texture it is useful to query the texture index of the current gradient by the indexOf method.
 */
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
    std::map<std::string, AbstractColorGradient *> m_gradients; /// The list of gradients with their name as lookup key
};

} // namespace gloperate

#include <gloperate/base/ColorGradientList.hpp>
