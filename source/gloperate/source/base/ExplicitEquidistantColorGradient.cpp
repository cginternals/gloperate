#include <gloperate/base/ExplicitEquidistantColorGradient.h>

#include <algorithm>

#include <glm/common.hpp>

namespace gloperate
{


ExplicitEquidistantColorGradient::ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const std::vector<reflectionzeug::Color> & colors)
: AbstractColorGradient(name)
, m_colors(colors)
, m_discrete(discrete)
{
}

ExplicitEquidistantColorGradient::ExplicitEquidistantColorGradient(const std::string & name, bool discrete, std::initializer_list<reflectionzeug::Color> colors)
: AbstractColorGradient(name)
, m_colors(colors.begin(), colors.end())
, m_discrete(discrete)
{
}

ExplicitEquidistantColorGradient::ExplicitEquidistantColorGradient(const std::string & name, bool discrete)
: AbstractColorGradient(name)
, m_discrete(discrete)
{
}

reflectionzeug::Color ExplicitEquidistantColorGradient::colorAt(float position) const
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
