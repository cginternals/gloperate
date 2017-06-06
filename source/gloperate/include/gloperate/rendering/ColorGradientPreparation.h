
#pragma once


#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <set>

#include <gloperate/gloperate_api.h>


namespace cppexpose
{
    class AbstractProperty;
}


namespace gloperate
{


class ColorGradientList;


/**
*  @brief
*    A utility class to provide convenience interfaces for common tasks with color gradients.
*
*    This class can generate icons (pixmaps) for color gradients and configure string properties to use these color gradient icons.
*/
class GLOPERATE_API ColorGradientPreparation
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] gradients
    *    The list of gradients
    *  @param[in] iconSize
    *    The size of the pixmaps as separate x and y dimensions
    */
    ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] gradients
    *    The list of gradients
    *  @param[in] iconSize
    *    The size of the pixmaps
    *  @param[in] whitelist
    *    The list of gradients names to consider
    *
    *  @remarks
    *    If the whitelist is empty, all gradients get processed.
    */
    ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize, const std::set<std::string> & whitelist);

    /**
    *  @brief
    *   Get the list of gradients
    *
    *  @return
    *    The list of gradients
    */
    const ColorGradientList & gradients() const;

    /**
    *  @brief
    *    Get the icon size
    *
    *  @return
    *    The icon size as separate x and y dimensions
    */
    const std::pair<std::uint32_t, std::uint32_t> & iconSize() const;

    /**
    *  @brief
    *    Get the list of gradient names
    *
    *  @return
    *    The list of gradient names
    *
    *  @remarks
    *    The order of gradients in the list is preserved.
    */
    std::vector<std::string> names() const;

    /**
    *  @brief
    *    Get the list pixmaps with pixel data
    *
    *  @return
    *    The list of pixmaps
    *
    *  @remarks
    *    The order of gradients in the list is preserved.
    */
    std::vector<std::vector<unsigned char>> pixmaps() const;

    /**
    *  @brief
    *    Append names of gradients to list of names
    *
    *  @param[in] names
    *    The list of names to fill
    *
    *  @remarks
    *    The list is emptied before appending names
    */
    void fillNames(std::vector<std::string> & names) const;

    /**
    *  @brief
    *    Append pixmaps to gradients to list of pixmaps
    *
    *  @param[in] pixmaps
    *    The list of pixmaps to fill
    *
    *  @remarks
    *    The list is emptied before appending names
    */
    void fillPixmaps(std::vector<std::vector<unsigned char>> & pixmaps) const;

    /**
    *  @brief
    *    Configure property to provide gradients with pixmaps as choices
    *
    *  @param[in] property
    *    The property to apply the options to
    *
    *  @remarks
    *    Especially useful for string properties.
    */
    void configureProperty(cppexpose::AbstractProperty * property) const;


protected:
    const ColorGradientList &               m_gradients; ///< The gradient list to operate on
    std::pair<std::uint32_t, std::uint32_t> m_iconSize;  ///< The icon size used for all generated pixmaps as separate x and y dimensions
    std::set<std::string>                   m_whitelist; ///< The names of the gradients to include into the property
};


} // namespace gloperate
