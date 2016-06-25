
#pragma once


#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


template <typename T>
DataSlot<T>::DataSlot(const T & value)
: cppexpose::DirectValue<T, AbstractDataSlot>(value)
{
}

template <typename T>
DataSlot<T>::~DataSlot()
{
}

template <typename T>
T & DataSlot<T>::operator*()
{
    return *this->ptr();
}

template <typename T>
const T & DataSlot<T>::operator*() const
{
    return *this->ptr();
}

template <typename T>
T * DataSlot<T>::operator->()
{
    return this->ptr();
}

template <typename T>
const T * DataSlot<T>::operator->() const
{
    return this->ptr();
}

template <typename T>
bool DataSlot<T>::isObject() const
{
    return false;
}


} // namespace gloperate
