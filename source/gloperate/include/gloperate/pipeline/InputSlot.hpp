
#pragma once


#include <gloperate/pipeline/InputSlot.h>

#include <cppexpose/typed/Typed.h>

#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Parameter.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/ProxyOutput.h>


namespace gloperate
{


template <typename T, typename BASE>
InputSlot<T, BASE>::InputSlot(Stage * parent, const std::string & name, const T & defaultValue)
: m_defaultValue(defaultValue)
, m_sourceType(SlotType::Empty)
{
    this->initProperty(parent, name);
}

template <typename T, typename BASE>
InputSlot<T, BASE>::~InputSlot()
{
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(Input<T> * source)
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
    this->connectionChanged();
    this->onValueChanged(m_source.input->value());

    // Success
    return true;
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(Parameter<T> * source)
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
    this->connectionChanged();
    this->onValueChanged(m_source.parameter->value());

    // Success
    return true;
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(Output<T> * source)
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
    this->connectionChanged();
    this->onValueChanged(m_source.output->value());

    // Success
    return true;
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(ProxyOutput<T> * source)
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
    this->connectionChanged();
    this->onValueChanged(m_source.proxyOutput->value());

    // Success
    return true;
}

template <typename T, typename BASE>
InputSlot<T, BASE> & InputSlot<T, BASE>::operator<<(Input<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T, typename BASE>
InputSlot<T, BASE> & InputSlot<T, BASE>::operator<<(Parameter<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T, typename BASE>
InputSlot<T, BASE> & InputSlot<T, BASE>::operator<<(Output<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T, typename BASE>
InputSlot<T, BASE> & InputSlot<T, BASE>::operator<<(ProxyOutput<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T, typename BASE>
const T & InputSlot<T, BASE>::operator*() const
{
    return *this->ptr();
}

template <typename T, typename BASE>
T * InputSlot<T, BASE>::operator->()
{
    return this->ptr();
}

template <typename T, typename BASE>
const AbstractSlot * InputSlot<T, BASE>::source() const
{
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input;
        case SlotType::Parameter:   return m_source.parameter;
        case SlotType::Output:      return m_source.output;
        case SlotType::ProxyOutput: return m_source.proxyOutput;
        default:                    return nullptr;
    }
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::isCompatible(const AbstractSlot * source) const
{
    if (source) {
        return this->type() == source->type();
    } else {
        return false;
    }
}

template <typename T, typename BASE>
bool InputSlot<T, BASE>::connect(AbstractSlot * source)
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

template <typename T, typename BASE>
void InputSlot<T, BASE>::disconnect()
{
    // Reset source property
    m_source.input       = nullptr;
    m_source.parameter   = nullptr;
    m_source.output      = nullptr;
    m_source.proxyOutput = nullptr;
    m_connection         = cppexpose::ScopedConnection();

    // Emit events
    this->connectionChanged();
    this->onValueChanged(m_defaultValue);
}

template <typename T, typename BASE>
cppexpose::AbstractTyped * InputSlot<T, BASE>::clone() const
{
    return nullptr;
}

template <typename T, typename BASE>
T InputSlot<T, BASE>::value() const
{
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input->value();
        case SlotType::Parameter:   return m_source.parameter->value();
        case SlotType::Output:      return m_source.output->value();
        case SlotType::ProxyOutput: return m_source.proxyOutput->value();
        default:                    return m_defaultValue;
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
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input->ptr();
        case SlotType::Parameter:   return m_source.parameter->ptr();
        case SlotType::Output:      return m_source.output->ptr();
        case SlotType::ProxyOutput: return m_source.proxyOutput->ptr();
        default:                    return &m_defaultValue;
    }
}

template <typename T, typename BASE>
T * InputSlot<T, BASE>::ptr()
{
    switch (m_sourceType) {
        case SlotType::Input:       return m_source.input->ptr();
        case SlotType::Parameter:   return m_source.parameter->ptr();
        case SlotType::Output:      return m_source.output->ptr();
        case SlotType::ProxyOutput: return m_source.proxyOutput->ptr();
        default:                    return &m_defaultValue;
    }
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
