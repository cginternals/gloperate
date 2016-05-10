
#pragma once


#include <gloperate/pipeline/InputSlot.h>

#include <cppexpose/typed/Typed.h>


namespace gloperate
{


template <typename T>
InputSlot<T>::InputSlot(cppexpose::PropertyGroup * parent, const std::string & name, const T & defaultValue)
: cppexpose::TypedProperty<T>(parent, name)
, m_defaultValue(defaultValue)
, m_source(nullptr)
{
}

template <typename T>
InputSlot<T>::~InputSlot()
{
}

template <typename T>
bool InputSlot<T>::connect(const Data<T> * source)
{
    // Check if source is valid
    if (!source) {
        return false;
    }

    // Get source data
    m_source = source;

    // Connect to data container
    m_connection = m_source->valueChanged.connect([this] (const T & value)
    {
        this->valueChanged(value);
    } );

    // Emit events
    this->connectionChanged();
    this->valueChanged(m_source->value());

    // Success
    return true;
}

template <typename T>
bool InputSlot<T>::isCompatible(const cppexpose::AbstractProperty * source) const
{
    if (source) {
        return this->type() == source->asTyped()->type();
    } else {
        return false;
    }
}

template <typename T>
const cppexpose::AbstractProperty * InputSlot<T>::source() const
{
    return m_source;
}

template <typename T>
bool InputSlot<T>::connect(const cppexpose::AbstractProperty * source)
{
    // Check if source is valid and compatible data container
    if (!source || !isCompatible(source))
    {
        return false;
    }

    // Connect to source data
    return connect(dynamic_cast< const Data<T> * >(source));
}

template <typename T>
void InputSlot<T>::disconnect()
{
    // Reset source property
    m_source     = nullptr;
    m_connection = cppexpose::ScopedConnection();

    // Emit events
    this->connectionChanged();
    this->valueChanged(m_defaultValue);
}

template <typename T>
cppexpose::AbstractTyped * InputSlot<T>::clone() const
{
    return nullptr;
}

template <typename T>
T InputSlot<T>::value() const
{
    if (m_source) {
        return m_source->value();
    } else {
        return m_defaultValue;
    }
}

template <typename T>
void InputSlot<T>::setValue(const T &)
{
    // Not supported for input slots!
}

template <typename T>
const T * InputSlot<T>::ptr() const
{
    if (m_source) {
        return m_source->ptr();
    } else {
        return nullptr;
    }
}

template <typename T>
T * InputSlot<T>::ptr()
{
    return nullptr;
}

template <typename T>
cppexpose::AbstractTyped * InputSlot<T>::asTyped()
{
    return static_cast<cppexpose::AbstractTyped *>(this);
}

template <typename T>
const cppexpose::AbstractTyped * InputSlot<T>::asTyped() const
{
    return static_cast<const cppexpose::AbstractTyped *>(this);
}

template <typename T>
bool InputSlot<T>::isGroup() const
{
    return false;
}


} // namespace gloperate
