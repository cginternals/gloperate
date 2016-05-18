
#pragma once


#include <gloperate/pipeline/Input.h>


namespace gloperate
{


template <typename T>
Input<T>::Input(Stage * parent, const std::string & name, const T & value)
: InputSlot<T, AbstractInput>(parent, name, value)
{
}

template <typename T>
Input<T>::~Input()
{
}


} // namespace gloperate
