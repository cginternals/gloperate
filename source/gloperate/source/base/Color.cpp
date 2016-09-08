
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
    m_bgra[0] = static_cast<unsigned char>(blue);
    m_bgra[1] = static_cast<unsigned char>(green);
    m_bgra[2] = static_cast<unsigned char>(red);
    m_bgra[3] = static_cast<unsigned char>(alpha);
}

Color::Color(glm::tvec4<unsigned char> bgra)
{
    m_bgra = bgra;
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
    return m_bgra[2];
}

void Color::setRed(int value)
{
    assert(0 <= value && value <= 255);
    m_bgra[2] = static_cast<unsigned char>(value);
}

int Color::green() const
{
    return m_bgra[1];
}

void Color::setGreen(int value)
{
    assert(0 <= value && value <= 255);
    m_bgra[1] = static_cast<unsigned char>(value);
}

int Color::blue() const
{
    return m_bgra[0];
}

void Color::setBlue(int value)
{
    assert(0 <= value && value <= 255);
    m_bgra[0] = static_cast<unsigned char>(value);
}

int Color::alpha() const
{
    return m_bgra[3];
}

void Color::setAlpha(int value)
{
    assert(0 <= value && value <= 255);
    m_bgra[3] = static_cast<unsigned char>(value);
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
    return Color(glm::mix(m_bgra, other.m_bgra, interpolationValue));
}


} // namespace gloperate
