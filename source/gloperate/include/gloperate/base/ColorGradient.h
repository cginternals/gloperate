#pragma once

#include <array>
#include <vector>

#include <reflectionzeug/base/Color.h>

#include <gloperate/gloperate_api.h>

namespace globjects
{

class Texture;

}

namespace gloperate
{

class ColorGradientList;

class GLOPERATE_API ColorGradient
{
public:
    explicit ColorGradient(const std::string & name);

    virtual ~ColorGradient();

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

class GLOPERATE_API StaticGradient : public ColorGradient
{
public:
    StaticGradient(const std::string & name, const reflectionzeug::Color & color);

protected:
    reflectionzeug::Color m_color;

protected:
    virtual void fillPixelData(unsigned char * data, size_t numPixels) const override;
    virtual reflectionzeug::Color colorAt(float position) const override;
};

class GLOPERATE_API LinearGradient : public ColorGradient
{
public:
    LinearGradient(const std::string & name, const reflectionzeug::Color & color1, const reflectionzeug::Color & color2);

protected:
    reflectionzeug::Color m_color1;
    reflectionzeug::Color m_color2;

protected:
    virtual reflectionzeug::Color colorAt(float position) const override;
};

} // namespace reflectionzeug

#include <gloperate/base/ColorGradient.hpp>
