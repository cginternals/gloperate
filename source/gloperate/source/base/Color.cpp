
#include <glm/glm.hpp>

#include <gloperate/base/Color.h>

#include <cassert>


namespace gloperate
{


Color::Color()
: m_v{0u}
{
}

Color::Color(unsigned int bgra)
: m_v{bgra}
{
}

Color::Color(int red, int green, int blue, int alpha)
{
    m_rgba.a = static_cast<unsigned char>(alpha);
    m_rgba.r = static_cast<unsigned char>(red);
    m_rgba.g = static_cast<unsigned char>(green);
    m_rgba.b = static_cast<unsigned char>(blue);
}

Color::Color(glm::tvec4<unsigned char> rgba)
{
    m_rgba = rgba;
}

bool Color::operator==(const Color & rhs) const
{
    return m_v == rhs.m_v;
}

bool Color::operator!=(const Color & rhs) const
{
    return !(*this == rhs);
}

int Color::red() const
{
    return m_rgba.r;
}

void Color::setRed(int value)
{
    assert(0 <= value && value <= 255);
    m_rgba.r = static_cast<unsigned char>(value);
}

int Color::green() const
{
    return m_rgba.g;
}

void Color::setGreen(int value)
{
    assert(0 <= value && value <= 255);
    m_rgba.g = static_cast<unsigned char>(value);
}

int Color::blue() const
{
    return m_rgba.b;
}

void Color::setBlue(int value)
{
    assert(0 <= value && value <= 255);
    m_rgba.b = static_cast<unsigned char>(value);
}

int Color::alpha() const
{
    return m_rgba.a;
}

void Color::setAlpha(int value)
{
    assert(0 <= value && value <= 255);
    m_rgba.a = static_cast<unsigned char>(value);
}

unsigned int Color::bgra() const
{
    return m_v;
}

void Color::setBgra(unsigned int bgra)
{
    m_v = bgra;
}

Color Color::interpolate(const Color & other, float interpolationValue) const
{
    return Color(glm::mix(m_rgba, other.m_rgba, interpolationValue));
}


} // namespace gloperate
