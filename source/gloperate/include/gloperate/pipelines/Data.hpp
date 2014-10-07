#pragma once

#include <gloperate/pipelines/Data.h>

namespace gloperate {
	
template <typename T>
Data<T>::Data()
: m_data()
{
}

template <typename T>
Data<T>::Data(const T & data)
: m_data(data)
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
const T & Data<T>::operator=(const T & value)
{
	m_data = value;
	invalidated();

	return value;
}

template <typename T>
void Data<T>::setData(const T & value)
{
    m_data = value;
    invalidated();
}
	
} // namespace gloperate
