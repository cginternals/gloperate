
#pragma once


#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template <typename T>
Data<T>::Data(cppexpose::PropertyGroup * parent, const std::string & name, const T & value)
: cppexpose::DynamicProperty<T>(parent, name, value)
, m_required(false)
{
}

template <typename T>
Data<T>::~Data()
{
}

template <typename T>
bool Data<T>::required() const
{
    return m_required;
}

template <typename T>
void Data<T>::setRequired(bool required)
{
    m_required = required;
}


} // namespace gloperate
