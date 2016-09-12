
#pragma once


namespace gloperate
{


template <typename... Args>
ColorGradientList::ColorGradientList(AbstractColorGradient * gradient, Args... args)
: ColorGradientList(args...)
{
    add(gradient);
}


} // namespace gloperate
