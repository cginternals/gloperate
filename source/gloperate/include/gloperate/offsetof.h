
#pragma once

#include <cstddef>

#define GLOPERATE_OFFSETOF(class,member) ((std::size_t) &(((class*)0)->member))

namespace gloperate
{


template <typename Class, typename Type>
std::ptrdiff_t offset(Type Class::*member)
{
    return reinterpret_cast<std::ptrdiff_t>(&(((Class*)0)->*member));
}


} // namespace gloperate
