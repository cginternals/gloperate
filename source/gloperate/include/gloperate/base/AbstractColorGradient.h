#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include <reflectionzeug/base/Color.h>

#include <gloperate/gloperate_api.h>

namespace globjects
{


class Texture;


} // namespace globjects

namespace gloperate
{


class ColorGradientList;

/**
 * @brief
 *    The base class for color gradients (i.e., a mapping of float values in the range of [0..1] to a reflectionzeug::Color).
 *
 *    This class provides convenience methods for accessing the binary image data of the gradient with specific widths (pixelData)
 *    and for using this color gradient in an OpenGL texture (generateTexture).
 *
 *    A color gradient is identified through its name and this name is mainly used by the ColorGradientList class.
 *
 *    \see ColorGradientList
 *    \see ExplicitEquidistantColorGradient
 */
class GLOPERATE_API AbstractColorGradient
{
public:
    explicit AbstractColorGradient(const std::string & name);

    /**
     * @brief Virtual destructor for subclasses.
     */
    virtual ~AbstractColorGradient();

    const std::string & name() const;
    void setName(const std::string & name);

    std::vector<unsigned char> pixelData(size_t numPixels) const;

    template <size_t numPixels>
    std::array<unsigned char, numPixels * sizeof(std::uint32_t)> pixelData() const;

    virtual void fillPixelData(unsigned char * data, size_t numPixels) const;
    virtual reflectionzeug::Color colorAt(float position) const = 0;

    globjects::Texture * generateTexture(size_t numPixels) const;

protected:
    std::string m_name; /// The identifier of a color gradient which is currently used as output string.
};


} // namespace reflectionzeug


#include <gloperate/base/AbstractColorGradient.hpp>
