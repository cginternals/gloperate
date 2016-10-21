
#include <glm/glm.hpp>

#include <gloperate/base/Color.h>

#include <cassert>


namespace gloperate
{


Color::Color()
: m_bgra{0u, 0u, 0u, 255u}
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

Color::Color(float red, float green, float blue, float alpha)
{
    m_bgra[0] = static_cast<unsigned char>(blue  * 255.0f);
    m_bgra[1] = static_cast<unsigned char>(green * 255.0f);
    m_bgra[2] = static_cast<unsigned char>(red   * 255.0f);
    m_bgra[3] = static_cast<unsigned char>(alpha * 255.0f);
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

unsigned int Color::bgra() const
{
    return m_v;
}

void Color::setBgra(unsigned int bgra)
{
    m_v = bgra;
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

float Color::redf() const
{
    return m_bgra[2] / 255.0f;
}

void Color::setRedf(float value)
{
    assert(0.0f <= value && value <= 1.0f);
    m_bgra[2] = static_cast<unsigned char>(value * 255.0f);
}

float Color::greenf() const
{
    return m_bgra[1] / 255.0f;
}

void Color::setGreenf(float value)
{
    assert(0.0f <= value && value <= 1.0f);
    m_bgra[1] = static_cast<unsigned char>(value * 255.0f);
}

float Color::bluef() const
{
    return m_bgra[0] / 255.0f;
}

void Color::setBluef(float value)
{
    assert(0.0f <= value && value <= 1.0f);
    m_bgra[0] = static_cast<unsigned char>(value * 255.0f);
}

float Color::alphaf() const
{
    return m_bgra[3] / 255.0f;
}

void Color::setAlphaf(float value)
{
    assert(0.0f <= value && value <= 1.0f);
    m_bgra[3] = static_cast<unsigned char>(value * 255.0f);
}

glm::vec3 Color::toVec3() const
{
    return glm::vec3(redf(), greenf(), bluef());
}

glm::vec4 Color::toVec4() const
{
    return glm::vec4(redf(), greenf(), bluef(), alphaf());
}

std::string Color::toHexString() const
{
    return "#" + hexFromValue(alpha()) + hexFromValue(red()) + hexFromValue(green()) + hexFromValue(blue());
}

bool Color::fromHexString(const std::string & hex)
{
    if (hex.size() < 1 || hex[0] != '#')
    {
        return false;
    }

    if (hex.size() == 9)
    {
        setRed  (valueFromHex(hex.substr(3, 2)));
        setGreen(valueFromHex(hex.substr(5, 2)));
        setBlue (valueFromHex(hex.substr(7, 2)));
        setAlpha(valueFromHex(hex.substr(1, 2)));
    }

    else if (hex.size() == 7)
    {
        setRed  (valueFromHex(hex.substr(1, 2)));
        setGreen(valueFromHex(hex.substr(3, 2)));
        setBlue (valueFromHex(hex.substr(5, 2)));
        setAlpha(255);
    }

    else if (hex.size() == 5)
    {
        setRed  (valueFromHex(hex.substr(2, 1) + hex.substr(2, 1)));
        setGreen(valueFromHex(hex.substr(3, 1) + hex.substr(3, 1)));
        setBlue (valueFromHex(hex.substr(4, 1) + hex.substr(4, 1)));
        setAlpha(valueFromHex(hex.substr(1, 1) + hex.substr(1, 1)));
    }

    else if (hex.size() == 4)
    {
        setRed  (valueFromHex(hex.substr(1, 1) + hex.substr(1, 1)));
        setGreen(valueFromHex(hex.substr(2, 1) + hex.substr(2, 1)));
        setBlue (valueFromHex(hex.substr(3, 1) + hex.substr(3, 1)));
        setAlpha(255);
    }

    else
    {
        return false;
    }

    return true;
}

Color Color::interpolate(const Color & other, float interpolationValue) const
{
    return Color(glm::mix(m_bgra, other.m_bgra, interpolationValue));
}

std::string Color::hexFromValue(int value)
{
    std::string digits = "0123456789ABCDEF";

         if (value >= 255) return "FF";
    else if (value <= 0)   return "00";
    else                   return digits.substr(value / 16, 1) + digits.substr(value % 16, 1);
}

int Color::valueFromHex(const std::string & hex)
{
    int value = 0;

    for (size_t i=0; i<hex.size(); i++)
    {
        value *= 16;

        char c = hex[i];
        value += hexValue(c);
    }

    return value;
}

int Color::hexValue(char c)
{
    switch (c)
    {
        case 'f':
        case 'F': return 15;
        case 'e':
        case 'E': return 14;
        case 'd':
        case 'D': return 13;
        case 'c':
        case 'C': return 12;
        case 'b':
        case 'B': return 11;
        case 'a':
        case 'A': return 10;
        case '9': return 9;
        case '8': return 8;
        case '7': return 7;
        case '6': return 6;
        case '5': return 5;
        case '4': return 4;
        case '3': return 3;
        case '2': return 2;
        case '1': return 1;
        default:  return 0;
    }
}


} // namespace gloperate
