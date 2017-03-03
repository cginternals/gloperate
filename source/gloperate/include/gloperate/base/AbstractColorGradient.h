
#pragma once


#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <memory>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


class Color;
class ColorGradientList;


/**
*  @brief
*    The base class for color gradients (i.e., a mapping of float values in the range of [0..1] to a Color).
*
*    This class provides convenience methods for accessing the binary image data of the gradient with specific widths (pixelData)
*    and for using this color gradient in an OpenGL texture (generateTexture).
*
*    A color gradient is identified through its name and this name is mainly used by the ColorGradientList class.
*
*  @see Color
*  @see ColorGradientList
*  @see ExplicitEquidistantColorGradient
*/
class GLOPERATE_API AbstractColorGradient
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    The name for identification within a ColorGradientList. Should be unique.
    */
    explicit AbstractColorGradient(const std::string & name);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractColorGradient();

    /**
    *  @brief
    *    Get name
    *
    *  @return
    *    The current name
    */
    const std::string & name() const;

    /**
    *  @brief
    *    Update name
    *
    *  @param[in] name
    *    The new name
    */
    void setName(const std::string & name);

    /**
    *  @brief
    *    Computes an GBRA8 encoded vector of the gradient with the passed number of colors.
    *
    *  @param[in] numColors
    *    Number of colors to compute
    *
    *  @return
    *    An GBRA8 encoded vector with the size 4 * numColors
    */
    std::vector<unsigned char> pixelData(size_t numColors) const;

    /**
    *  @brief
    *    Computes an GBRA8 encoded vector of the gradient with the passed number of colors.
    *
    *  @param[in] numColors
    *    Number of colors to compute
    *
    *  @return
    *    An GBRA8 encoded vector with the size 4 * numColors
    */
    template <size_t numColors>
    std::array<unsigned char, numColors * sizeof(std::uint32_t)> pixelData() const;

    /**
    *  @brief
    *    Computes GBRA8 encoded colors and store the resulting color list at the passed address
    *
    *  @param[in] data
    *    Address to store the color list at
    *  @param[in] numColors
    *    Number of colors to compute and store
    *
    *  @remarks
    *    This method can be overriden in subclasses to implement optimized color computation algorithms.
    *    By default, the method computes a new color for each pixel.
    */
    virtual void fillPixelData(unsigned char * data, size_t numColors) const;

    /**
    *  @brief
    *    Return the associated color for a given virtual position within the gradient
    *
    *  @param[in] position
    *    The position in [0..1] within the gradient
    *
    *  @return
    *    The color
    */
    virtual Color colorAt(float position) const = 0;

    /**
    *  @brief
    *    Computes a fully initialized 1D texture containing the gradient
    *
    *  @param[in] numColors
    *    Number of colors in the resulting texture (i.e., number of texels).
    *
    *  @return
    *    The 1D texture
    */
    std::unique_ptr<globjects::Texture> generateTexture(size_t numColors) const;


protected:
    std::string m_name; ///< The identifier of a color gradient.
};


} // namespace gloperate


#include <gloperate/base/AbstractColorGradient.inl>
