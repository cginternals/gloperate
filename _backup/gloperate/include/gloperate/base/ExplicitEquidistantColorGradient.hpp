#pragma once

#include <gloperate/base/ExplicitEquidistantColorGradient.h>

namespace gloperate
{


template <size_t Count>
ExplicitEquidistantColorGradient::ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const std::array<reflectionzeug::Color, Count> & colors)
: AbstractColorGradient(name)
, m_colors(colors.begin(), colors.end())
, m_discrete(discrete)
{
}

template <typename... Args>
ExplicitEquidistantColorGradient::ExplicitEquidistantColorGradient(const std::string & name, bool discrete, const reflectionzeug::Color & color, Args... args)
: ExplicitEquidistantColorGradient(name, discrete, args...)
{
    m_colors.insert(m_colors.begin(), color);
}


} // namespace gloperate
