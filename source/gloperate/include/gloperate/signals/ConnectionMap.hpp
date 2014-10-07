
#pragma once

#include <gloperate/signals/ConnectionMap.h>

namespace gloperate
{

template <typename T>
Connection & ConnectionMap::operator[](T * object)
{
	return get(static_cast<void *>(object));
}

} // namespace gloperate
