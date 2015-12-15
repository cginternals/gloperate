#pragma once

#include <gloperate/base/ColorGradientList.h>

namespace gloperate
{

template <typename... Args>
ColorGradientList::ColorGradientList(AbstractColorGradient * gradient, Args... args)
: ColorGradientList(args...)
{
    add(gradient);
}

} // namespace gloperate
