
#pragma once


#include <gloperate/pipeline/Output.h>


namespace gloperate
{


template <typename T>
Output<T>::Output(Stage * parent, const std::string & name, const T & value)
: Data<T, AbstractOutput>(parent, name, value)
{
}

template <typename T>
Output<T>::~Output()
{
}


} // namespace gloperate
