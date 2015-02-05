
#pragma once

#include <gloperate/base/ConnectionMap.h>


namespace gloperate
{

template <typename T>
Connection & ConnectionMap::operator[](T * object)
{
    return get(static_cast<void *>(object));
}

} // namespace gloperate
