
#pragma once


#include <gloperate/pipeline/Parameter.h>


namespace gloperate
{


template <typename T>
Parameter<T>::Parameter(Stage * parent, const std::string & name, const T & value)
: Data<T, AbstractParameter>(parent, name, value)
{
    this->initParameter(parent);
}

template <typename T>
Parameter<T>::~Parameter()
{
}


} // namespace gloperate
