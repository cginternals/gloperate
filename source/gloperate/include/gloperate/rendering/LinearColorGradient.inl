
#pragma once


namespace gloperate
{


template <size_t Count>
LinearColorGradient::LinearColorGradient(const std::string & name, bool discrete, const std::array<Color, Count> & colors)
: AbstractColorGradient(name)
, m_colors(colors.begin(), colors.end())
, m_discrete(discrete)
{
}

template <typename... Args>
LinearColorGradient::LinearColorGradient(const std::string & name, bool discrete, const Color & color, Args... args)
: LinearColorGradient(name, discrete, args...)
{
    m_colors.insert(m_colors.begin(), color);
}


} // namespace gloperate
