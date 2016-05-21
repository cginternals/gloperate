
#pragma once


#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template <typename T, typename BASE>
Data<T, BASE>::Data(Stage * parent, const std::string & name, const T & value)
: cppexpose::DynamicProperty<T, BASE>(parent, name, value)
{
}

template <typename T, typename BASE>
Data<T, BASE>::~Data()
{
}

template <typename T, typename BASE>
T & Data<T, BASE>::operator*()
{
    return *this->ptr();
}

template <typename T, typename BASE>
const T & Data<T, BASE>::operator*() const
{
    return *this->ptr();
}

template <typename T, typename BASE>
T * Data<T, BASE>::operator->()
{
    return this->ptr();
}

template <typename T, typename BASE>
const T * Data<T, BASE>::operator->() const
{
    return this->ptr();
}


} // namespace gloperate
