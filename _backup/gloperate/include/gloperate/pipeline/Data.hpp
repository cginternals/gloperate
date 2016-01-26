
#pragma once


#include <gloperate/pipeline/Data.h>


namespace gloperate 
{


template <typename T>
template <typename... Args>
Data<T>::Data(Args&&... args)
: m_data(std::forward<Args>(args)...)
{
}

template <typename T>
T & Data<T>::data()
{
    return m_data;
}

template <typename T>
const T & Data<T>::data() const
{
    return m_data;
}

template <typename T>
T & Data<T>::operator*()
{
    return m_data;
}

template <typename T>
const T & Data<T>::operator*() const
{
    return m_data;
}

template <typename T>
T * Data<T>::operator->()
{
    return &m_data;
}

template <typename T>
const T * Data<T>::operator->() const
{
    return &m_data;
}

template <typename T>
Data<T>::operator const T &() const
{
    return m_data;
}

template <typename T>
Data<T> & Data<T>::operator=(const Data<T> & data)
{
    *this = data.data();

    return *this;
}

template <typename T>
const T & Data<T>::operator=(const T & value)
{
    m_data = value;
    invalidate();

    return value;
}

template <typename T>
void Data<T>::setData(const T & value)
{
    m_data = value;
    invalidate();
}

template <typename T>
std::string Data<T>::type() const 
{
    return typeid(T).name(); 
}

    
} // namespace gloperate
