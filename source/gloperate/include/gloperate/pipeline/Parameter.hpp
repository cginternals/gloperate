
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

template <typename T>
bool Parameter<T>::isValid() const
{
    // Parameters are always valid
    return true;
}


} // namespace gloperate
