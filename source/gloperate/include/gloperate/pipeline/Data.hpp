
#pragma once


#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template <typename T>
Data<T>::Data(cppexpose::PropertyGroup * parent, const std::string & name, const T & value)
: cppexpose::DynamicProperty<T>(parent, name, value)
, AbstractData()
{
}

template <typename T>
Data<T>::Data(Stage * parent, const std::string & name, const T & value)
: cppexpose::DynamicProperty<T>(parent, name, value)
, AbstractData(parent)
{
}

template <typename T>
Data<T>::~Data()
{
}


} // namespace gloperate
