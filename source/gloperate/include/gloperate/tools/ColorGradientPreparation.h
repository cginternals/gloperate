#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <set>

#include <gloperate/gloperate_api.h>


namespace reflectionzeug
{


class AbstractProperty;


} // namespace reflectionzeug


namespace gloperate
{


class ColorGradientList;

/**
 * @brief
 *    A utility class to provide convenience interfaces for common tasks with color gradients.
 *
 *    This class can generate icons for color gradients and configure string properties to use these color gradient icons.
 */
class GLOPERATE_API ColorGradientPreparation
{
public:
    /**
     * @brief ColorGradientPreparation
     *   The constructor
     * @param gradients [in]
     *  The list of gradients to operate on.
     * @param iconSize [in]
     *  The size of the pixmaps that should get generated and configured as the pixmaps for a property.
     * @param whitelist [in]
     *  An optional list of gradients names to consider while processing the gradient list. If the list is empty, all gradients get processed.
     */
    ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize);
    ColorGradientPreparation(const ColorGradientList & gradients, const std::pair<std::uint32_t, std::uint32_t> & iconSize, const std::set<std::string> & whitelist);

    /**
     * @brief Returns the passed gradient list from the constructor.
     */
    const ColorGradientList & gradients() const;

    /**
     * @brief Returns the passed icon size from the constructor.
     */
    const std::pair<std::uint32_t, std::uint32_t> & iconSize() const;

    /**
     * @brief Extracts and returns a list of all gradient names from the gradient list (the order of the gradient list is preserved).
     * @return the names of all gradients.
     */
    std::vector<std::string> names() const;

    /**
     * @brief Extracts and returns a list of pixmaps of all gradients from the gradient list with the icon size passed in the constructor (the order of the gradient list is preserved).
     * @return the pixmaps to all gradients.
     */
    std::vector<std::vector<unsigned char>> pixmaps() const;

    /**
     * @brief Extracts and stores a list of names of each gradient into the passed parameter.
     * @param names the parameter to store the list of names into (is cleared before inserting).
     */
    void fillNames(std::vector<std::string> & names) const;

    /**
     * @brief Extracts and stores a list of pixmaps of each gradient into the passed parameter.
     * @param names the parameter to store the list of pixmaps into (is cleared before inserting).
     */
    void fillPixmaps(std::vector<std::vector<unsigned char>> & pixmaps) const;

    /**
     * @brief Utility method to set the choices, iconSize and pixmaps options on the given property.
     *     Especially useful for string properties.
     * @param property the property to apply the options to.
     */
    void configureProperty(reflectionzeug::AbstractProperty * property) const;
protected:
    const ColorGradientList & m_gradients; /// The gradient list to operate on
    std::pair<std::uint32_t, std::uint32_t> m_iconSize; /// The icon size used for all generated pixmaps
    std::set<std::string> m_whitelist; /// The names of the gradients to include into the property
};


} // namespace gloperate
