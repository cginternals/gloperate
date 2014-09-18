#pragma once

#include <gloperate/pipelines/Parameter.h>

namespace gloperate {

template <typename T>
Parameter<T>::Parameter()
{
}

template <typename T>
Parameter<T>::Parameter(const T & data)
: Data<T>(data)
{
}

template <typename T>
const T & Parameter<T>::operator=(const T & value)
{
	return Data<T>::operator=(value);
}

} // namespace gloperate
