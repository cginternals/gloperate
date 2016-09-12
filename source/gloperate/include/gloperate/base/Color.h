
#pragma once


#include <glm/vec4.hpp>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    A four component ubyte color class (red, green, blue and alpha).
*
*    Internally, the components are stored in the format BGRA to be complient to the Qt QColor.
*/
class GLOPERATE_API Color
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *    Constructs a Color with all components at '0'
    */
    Color();

    /**
    *  @brief
    *    Constructor
    *
    *    Constructs a color with given component values
    *
    *  @param[in] bgra
    *    The component values
    */
    explicit Color(unsigned int bgra);

    /**
    *  @brief
    *    Constructor
    *
    *    Constructs a color with given component values
    *
    *  @param[in] red
    *    The red component value [0..255]
    *  @param[in] green
    *    The green component value [0..255]
    *  @param[in] blue
    *    The blue component value [0..255]
    *  @param[in] alpha
    *    The alpha component value [0..255] (optional)
    */
    Color(int red, int green, int blue, int alpha = 255);

    /**
    *  @brief
    *    Constructor
    *
    *    Constructs a color with given component values
    *
    *  @param[in] bgra
    *    The component values
    */
    Color(glm::tvec4<unsigned char> bgra);

    /**
    *  @brief
    *    Compare for equality
    *
    *  @param[in] rhs
    *    The other color
    *
    *  @return
    *    'true' if all component values are equal, else 'false'
    */
    bool operator==(const Color & rhs) const;

    /**
    *  @brief
    *    Compare for inequality
    *
    *  @param[in] rhs
    *    The other color
    *
    *  @return
    *    'true' if any component values differs, else 'false'
    */
    bool operator!=(const Color & rhs) const;

    /**
    *  @brief
    *    Get the component values as unsigned int
    *
    *  @return
    *    An unsigned int representation of the color
    */
    unsigned int bgra() const;

    /**
    *   @brief
    *     Update all component values
    *
    *   @param[in] bgra
    *     The component values
    */
    void setBgra(unsigned int bgra);

    /**
    *  @brief
    *    Get the red component value
    *
    *  @return
    *    The red component value
    */
    int red() const;

    /**
    *  @brief
    *    Update the red component value
    *
    *  @param[in] value
    *    The red component value
    */
    void setRed(int value);

    /**
    *  @brief
    *    Get the green component value
    *
    *  @return
    *    The green component value
    */
    int green() const;

    /**
    *  @brief
    *    Update the green component value
    *
    *  @param[in] value
    *    The green component value
    */
    void setGreen(int value);

    /**
    *  @brief
    *    Get the blue component value
    *
    *  @return
    *    The blue component value
    */
    int blue() const;

    /**
    *  @brief
    *    Update the blue component value
    *
    *  @param[in] value
    *    The blue component value
    */
    void setBlue(int value);

    /**
    *  @brief
    *    Get the alpha component value
    *
    *  @return
    *    The alpha component value
    */
    int alpha() const;

    /**
    *  @brief
    *    Update the alpha component value
    *
    *  @param[in] value
    *    The alpha component value
    */
    void setAlpha(int value);

    /**
    *  @brief
    *    Get mixed color
    *
    *  @param[in] other
    *    The color to mix with
    *  @param[in] interpolationValue
    *    Value for the ratio of the colors
    *
    *  @return
    *    The mixed color
    */
    Color interpolate(const Color & other, float interpolationValue) const;


protected:
    union
    {
        glm::tvec4<unsigned char> m_bgra; ///< Per component representation of the color
        unsigned int              m_v;    ///< GBRA representation of the color
    };
};


} // namespace gloperate
