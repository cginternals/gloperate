
#pragma once


#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template <typename T>
Data<T>::Data(Stage * parent, const std::string & name, const T & value)
: cppexpose::DynamicProperty<T, AbstractData>(parent, name, value)
{
    this->initData(parent);
}

template <typename T>
Data<T>::~Data()
{
}


} // namespace gloperate
