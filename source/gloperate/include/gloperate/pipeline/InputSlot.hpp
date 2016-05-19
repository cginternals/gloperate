
#pragma once


#include <gloperate/pipeline/InputSlot.h>

#include <cppexpose/typed/Typed.h>


namespace gloperate
{


template <typename T, typename BASE>
InputSlot<T, BASE>::InputSlot(Stage * parent, const std::string & name, const T & defaultValue)
: m_defaultValue(defaultValue)
, m_source(nullptr)
{
    this->initProperty(parent, name);
}

template <typename T, typename BASE>
InputSlot<T, BASE>::~InputSlot()
{
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(const Output<T> * source)
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

template <typename T, typename BASE>
bool InputSlot<T, BASE>::isCompatible(const cppexpose::AbstractProperty * source) const
{
    if (source) {
        return this->type() == source->type();
    } else {
        return false;
    }
}

template <typename T, typename BASE>
const cppexpose::AbstractProperty * InputSlot<T, BASE>::source() const
{
    return m_source;
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(const cppexpose::AbstractProperty * source)
{
    // Check if source is valid and compatible data container
    if (!source || !isCompatible(source))
    {
        return false;
    }

    // Connect to source data
    return connect(dynamic_cast< const Output<T> * >(source));
}

template <typename T, typename BASE>
void InputSlot<T, BASE>::disconnect()
{
    // Reset source property
    m_source     = nullptr;
    m_connection = cppexpose::ScopedConnection();

    // Emit events
    this->connectionChanged();
    this->valueChanged(m_defaultValue);
}

template <typename T, typename BASE>
cppexpose::AbstractTyped * InputSlot<T, BASE>::clone() const
{
    return nullptr;
}

template <typename T, typename BASE>
T InputSlot<T, BASE>::value() const
{
    if (m_source) {
        return m_source->value();
    } else {
        return m_defaultValue;
    }
}

template <typename T, typename BASE>
void InputSlot<T, BASE>::setValue(const T &)
{
    // Not supported for input slots!
}

template <typename T, typename BASE>
const T * InputSlot<T, BASE>::ptr() const
{
    if (m_source) {
        return m_source->ptr();
    } else {
        return nullptr;
    }
}

template <typename T, typename BASE>
T * InputSlot<T, BASE>::ptr()
{
    return nullptr;
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::isGroup() const
{
    return false;
}

template <typename T, typename BASE>
void InputSlot<T, BASE>::onValueChanged(const T & value)
{
    this->valueChanged(value);
}


} // namespace gloperate
