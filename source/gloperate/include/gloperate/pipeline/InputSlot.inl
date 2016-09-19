
#pragma once


#include <cppexpose/typed/Typed.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Parameter.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/ProxyOutput.h>


namespace gloperate
{


template <typename T>
InputSlot<T>::InputSlot(const T & value)
: m_defaultValue(value)
, m_sourceType(SlotType::Empty)
{
    m_source.slot = nullptr;
}

template <typename T>
InputSlot<T>::~InputSlot()
{
}

template <typename T>
bool InputSlot<T>::connect(Input<T> * source)
{
    // Check if source is valid
    if (!source) {
        return false;
    }

    // Get source data
    m_source.input = source;
    m_sourceType = SlotType::Input;

    // Connect to data container
    m_connection = m_source.input->valueChanged.connect([this] (const T & value)
    {
        this->onValueChanged(value);
    } );

    // Emit events
    this->promoteConnection();
    this->promoteRequired();
    this->onValueChanged(m_source.input->value());

    // Success
    return true;
}

template <typename T>
bool InputSlot<T>::connect(Parameter<T> * source)
{
    // Check if source is valid
    if (!source) {
        return false;
    }

    // Get source data
    m_source.parameter = source;
    m_sourceType = SlotType::Parameter;

    // Connect to data container
    m_connection = m_source.parameter->valueChanged.connect([this] (const T & value)
    {
        this->onValueChanged(value);
    } );

    // Emit events
    this->promoteConnection();
    this->promoteRequired();
    this->onValueChanged(m_source.parameter->value());

    // Success
    return true;
}

template <typename T>
bool InputSlot<T>::connect(Output<T> * source)
{
    // Check if source is valid
    if (!source) {
        return false;
    }

    // Get source data
    m_source.output = source;
    m_sourceType = SlotType::Output;

    // Connect to data container
    m_connection = m_source.output->valueChanged.connect([this] (const T & value)
    {
        this->onValueChanged(value);
    } );

    // Emit events
    this->promoteConnection();
    this->promoteRequired();
    this->onValueChanged(m_source.output->value());

    // Success
    return true;
}

template <typename T>
bool InputSlot<T>::connect(ProxyOutput<T> * source)
{
    // Check if source is valid
    if (!source) {
        return false;
    }

    // Get source data
    m_source.proxyOutput = source;
    m_sourceType = SlotType::ProxyOutput;

    // Connect to data container
    m_connection = m_source.proxyOutput->valueChanged.connect([this] (const T & value)
    {
        this->onValueChanged(value);
    } );

    // Emit events
    this->promoteConnection();
    this->promoteRequired();
    this->onValueChanged(m_source.proxyOutput->value());

    // Success
    return true;
}

template <typename T>
InputSlot<T> & InputSlot<T>::operator<<(Input<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T>
InputSlot<T> & InputSlot<T>::operator<<(Parameter<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T>
InputSlot<T> & InputSlot<T>::operator<<(Output<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T>
InputSlot<T> & InputSlot<T>::operator<<(ProxyOutput<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T>
const T & InputSlot<T>::operator*() const
{
    return *this->ptr();
}

template <typename T>
T * InputSlot<T>::operator->()
{
    return this->ptr();
}

template <typename T>
const AbstractSlot * InputSlot<T>::source() const
{
    return m_source.slot;
}

template <typename T>
bool InputSlot<T>::isCompatible(const AbstractSlot * source) const
{
    if (source) {
        return this->type() == source->type();
    } else {
        return false;
    }
}

template <typename T>
bool InputSlot<T>::connect(AbstractSlot * source)
{
    // Check if source is valid and compatible data container
    if (!source || !isCompatible(source))
    {
        return false;
    }

    // Connect to source data
    switch (source->slotType())
    {
        case SlotType::Input:
            return connect(static_cast< Input<T> * >(source));

        case SlotType::Parameter:
            return connect(static_cast< Parameter<T> * >(source));

        case SlotType::Output:
            return connect(static_cast< Output<T> * >(source));

        case SlotType::ProxyOutput:
            return connect(static_cast< ProxyOutput<T> * >(source));

        default:
            return false;
    }
}

template <typename T>
void InputSlot<T>::disconnect()
{
    // Reset source property
    m_source.slot = nullptr;
    m_sourceType  = SlotType::Empty;
    m_connection  = cppexpose::ScopedConnection();

    // Emit events
    this->promoteConnection();
    this->onValueChanged(m_defaultValue);
}

template <typename T>
cppexpose::AbstractTyped * InputSlot<T>::clone() const
{
    return nullptr;
}

template <typename T>
T InputSlot<T>::value() const
{
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input->value();
        case SlotType::Parameter:   return m_source.parameter->value();
        case SlotType::Output:      return m_source.output->value();
        case SlotType::ProxyOutput: return m_source.proxyOutput->value();
        default:                    return m_defaultValue;
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
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input->ptr();
        case SlotType::Parameter:   return m_source.parameter->ptr();
        case SlotType::Output:      return m_source.output->ptr();
        case SlotType::ProxyOutput: return m_source.proxyOutput->ptr();
        default:                    return &m_defaultValue;
    }
}

template <typename T>
T * InputSlot<T>::ptr()
{
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input->ptr();
        case SlotType::Parameter:   return m_source.parameter->ptr();
        case SlotType::Output:      return m_source.output->ptr();
        case SlotType::ProxyOutput: return m_source.proxyOutput->ptr();
        default:                    return &m_defaultValue;
    }
}

template <typename T>
bool InputSlot<T>::isValid() const
{
    if (m_sourceType == SlotType::Empty)
    {
        return false;
    }

    return m_source.slot->isValid();
}

template <typename T>
void InputSlot<T>::onRequiredChanged()
{
    promoteRequired();
}

template <typename T>
bool InputSlot<T>::isObject() const
{
    return false;
}

template <typename T>
void InputSlot<T>::promoteConnection()
{
    // Emit signal
    this->connectionChanged();
    this->parentStage()->invalidateInputConnections();
}

template <typename T>
void InputSlot<T>::promoteRequired()
{
    // Check if input slot is connected
    if (m_sourceType == SlotType::Empty)
    {
        return;
    }

    // Promote required-flag to connected slot
    m_source.slot->setRequired(this->m_required);
}


} // namespace gloperate
