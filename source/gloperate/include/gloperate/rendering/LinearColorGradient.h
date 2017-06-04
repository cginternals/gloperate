
#pragma once


#include <array>
#include <vector>
#include <initializer_list>

#include <gloperate/rendering/Color.h>
#include <gloperate/rendering/AbstractColorGradient.h>


namespace gloperate
{


/**
*  @brief
*    A specific color gradient with equidistant key colors.
*
*    The gradient can be both discrete or continuous.
*/
class GLOPERATE_API LinearColorGradient : public AbstractColorGradient
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    The name
    *  @param[in] discrete
    *    Whether the gradient should be discrete or continuous
    *  @param[in] colors
    *    The list of colors
    */
    LinearColorGradient(const std::string & name, bool discrete, const std::vector<Color> & colors);

    /**
    *  @brief
    *    Constructor
    *
    *  @tparam Count
    *    The number of colors
    *  @param[in] name
    *    The name
    *  @param[in] discrete
    *    Whether the gradient should be discrete or continuous
    *  @param[in] colors
    *    The list of colors
    */
    template <size_t Count>
    LinearColorGradient(const std::string & name, bool discrete, const std::array<Color, Count> & colors);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    The name
    *  @param[in] discrete
    *    Whether the gradient should be discrete or continuous
    *  @param[in] colors
    *    The list of colors
    */
    LinearColorGradient(const std::string & name, bool discrete, std::initializer_list<Color> colors);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    The name
    *  @param[in] discrete
    *    Whether the gradient should be discrete or continuous
    *  @param[in] color
    *    Head of the list of colors
    *  @param[in] args
    *    Tail of the list of colors
    */
    template <typename... Args>
    LinearColorGradient(const std::string & name, bool discrete, const Color & color, Args... args);


protected:
    std::vector<Color> m_colors;   ///< The list of colors
    bool               m_discrete; ///< Whether this gradient is discrete ('true') or continuous ('false')


protected:
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
    virtual Color colorAt(float position) const override;

    /**
    *  @brief
    *    Constructor for empty list of colors
    *
    *  @param[in] name
    *    The name
    *  @param[in] discrete
    *    Whether the gradient should be discrete or continuous
    */
    LinearColorGradient(const std::string & name, bool discrete);
};


} // namespace gloperate


#include <gloperate/rendering/LinearColorGradient.inl>
