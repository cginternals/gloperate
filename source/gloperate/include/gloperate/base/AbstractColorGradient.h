#pragma once

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

class GLOPERATE_API AbstractColorGradient
{
public:
    explicit AbstractColorGradient(const std::string & name);

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
    std::string m_name;
};


} // namespace reflectionzeug

#include <gloperate/base/AbstractColorGradient.hpp>
