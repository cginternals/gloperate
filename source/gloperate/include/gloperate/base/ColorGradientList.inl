
#pragma once


namespace gloperate
{


template <typename... Args>
ColorGradientList::ColorGradientList(AbstractColorGradient * gradient, Args... args)
: ColorGradientList(args...)
{
    add(std::unique_ptr<AbstractColorGradient>(gradient));
}


} // namespace gloperate
