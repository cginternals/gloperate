#pragma once

#include <gloperate/pipeline/InputSlot.h>

#include <typeinfo>


namespace gloperate 
{

template <typename T>
const T InputSlot<T>::s_defaultValue = T();

template <typename T>    
InputSlot<T>::InputSlot()
: m_data(nullptr)
{
}

template <typename T>
const T & InputSlot<T>::data() const
{
    return data(s_defaultValue);
}

template <typename T>
const T & InputSlot<T>::data(const T & defaultValue) const
{
    return isConnected() ? m_data->data() : defaultValue;
}
    
template <typename T>
const T & InputSlot<T>::operator*() const
{
    return data();
}
    
template <typename T>
const T * InputSlot<T>::operator->() const
{
    return &data();
}

template <typename T>
bool InputSlot<T>::connectTo(const AbstractData & data)
{
    const Data<T> * data_ptr = dynamic_cast<const Data<T>*>(&data);
    
    if (!data_ptr)
    {
        printIncompatibleMessage(this, typeid(T).name(), data);
        return false;
    }
    
    connect(*data_ptr);
    
    return true;
}

template <typename T>
bool InputSlot<T>::matchType(const AbstractData & data)
{
    const Data<T> * data_ptr = dynamic_cast<const Data<T>*>(&data);

    return data_ptr != nullptr;
}

template <typename T>
const Data<T> & InputSlot<T>::operator=(const Data<T> & data)
{
    connect(data);

    return data;
}

template <typename T>
InputSlot<T> & InputSlot<T>::operator=(InputSlot<T> & slot)
{
    if (slot.isConnected())
    {
        connect(*slot.m_data);
    }

    return *this;
}

template <typename T>
void InputSlot<T>::connect(const Data<T> & data)
{
    m_data = &data;
    m_connection = data.invalidated.connect([this]() { this->changed(); });
    connectionChanged();
}

template <typename T>
const AbstractData * InputSlot<T>::connectedData() const
{
    return m_data;
}

} // namespace gloperate
