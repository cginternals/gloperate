
#pragma once


#include <string>

#include <glm/vec4.hpp>

#include <gloperate/gloperate_api.h>

namespace gloperate
{


/**
*  @brief
*    A simple four component color class (red, green, blue and alpha).
*
*    Internally, the components are stored in the format GBRA to be complient to Qt.
*/
class GLOPERATE_API Color
{
public:
    Color();
    explicit Color(unsigned int bgra);
    Color(int red, int green, int blue, int alpha = 255);
    Color(glm::tvec4<unsigned char> rgba);

    bool operator==(const Color & rhs) const;
    bool operator!=(const Color & rhs) const;

    unsigned int bgra() const;
    void setBgra(unsigned int bgra);

    int red() const;
    void setRed(int value);
    int green() const;
    void setGreen(int value);
    int blue() const;
    void setBlue(int value);
    int alpha() const;
    void setAlpha(int value);

    Color interpolate(const Color & other, float interpolationValue) const;

protected:
    union
    {
        glm::tvec4<unsigned char> m_rgba;
        unsigned int m_v;
    };
};


} // namespace gloperate
