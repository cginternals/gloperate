
#include <gloperate/rendering/LinearColorGradient.h>

#include <algorithm>

// [TODO]: Workaround until https://github.com/g-truc/glm/pull/856
#include <glm/detail/type_vec1.hpp>
#include <glm/common.hpp>


namespace gloperate
{


LinearColorGradient::LinearColorGradient(const std::string & name, bool discrete, const std::vector<Color> & colors)
: AbstractColorGradient(name)
, m_colors(colors)
, m_discrete(discrete)
{
}

LinearColorGradient::LinearColorGradient(const std::string & name, bool discrete, std::initializer_list<Color> colors)
: AbstractColorGradient(name)
, m_colors(colors.begin(), colors.end())
, m_discrete(discrete)
{
}

LinearColorGradient::LinearColorGradient(const std::string & name, bool discrete)
: AbstractColorGradient(name)
, m_discrete(discrete)
{
}

Color LinearColorGradient::colorAt(float position) const
{
    if (m_discrete)
    {
        float offset = position * float(m_colors.size());

        int index = glm::min(int(glm::floor(offset)), int(m_colors.size()-1));

        return m_colors.at(index);
    }
    else
    {
        float offset = position * float(m_colors.size()-1);

        int lowerIndex = glm::min(int(glm::floor(offset)), int(m_colors.size()-1));
        int upperIndex = glm::min(int(glm::ceil(offset)), int(m_colors.size()-1));

        return m_colors.at(lowerIndex).interpolate(m_colors.at(upperIndex), glm::fract(offset));
    }
}


} // namespace gloperate
