
#pragma once


#include <cppexpose/typed/Typed.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Slot.h>


namespace gloperate
{

template <typename T>
template <typename U>
auto Slot<T>::DereferenceHelper<U>::pointer(U * value) -> Pointer
{
    return value;
}

template <typename T>
template <typename U>
auto Slot<T>::DereferenceHelper<U*>::pointer(U ** value) -> Pointer
{
    return *value;
}


template <typename T>
Slot<T>::Slot(SlotType slotType, const std::string & name, Stage * parent, const T & value, bool dynamic)
: cppexpose::DirectValue<T, AbstractSlot>(value)
, m_valid(true)
, m_dynamic(dynamic)
, m_source(nullptr)
{
    // Do not add property to object, yet. Just initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Initialize slot, will also add slot as a property
    this->initSlot(slotType, parent, cppexpose::PropertyOwnership::None);
}

template <typename T>
Slot<T>::Slot(SlotType slotType, const std::string & name, const T & value, bool dynamic)
: cppexpose::DirectValue<T, AbstractSlot>(value)
, m_valid(true)
, m_dynamic(dynamic)
, m_source(nullptr)
{
    // Do not add property to object, yet. Just initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Initialize slot
    this->initSlot(slotType, nullptr, cppexpose::PropertyOwnership::None);
}

template <typename T>
Slot<T>::~Slot()
{
}

template <typename T>
bool Slot<T>::connect(Slot<T> * source)
{
    // Check if source is valid
    if (!source) {
        return false;
    }

    // Set source
    m_source = source;

    // Connect to data container
    m_connection = m_source->valueChanged.connect([this] (const T & value)
    {
        this->onValueChanged(value);
    } );

    // Emit events
    this->promoteConnection();
    this->promoteRequired();
    this->onValueChanged(m_source->value());

    // Success
    return true;
}

template <typename T>
Slot<T> & Slot<T>::operator<<(Slot<T> & source)
{
    this->connect(&source);
    return *this;
}

template <typename T>
const T & Slot<T>::operator*() const
{
    return *this->ptr();
}

template <typename T>
auto Slot<T>::operator->() -> typename DereferenceHelper<T>::Pointer
{
    return DereferenceHelper<T>::pointer(this->ptr());
}

template <typename T>
auto Slot<T>::operator->() const -> typename DereferenceHelper<const T>::Pointer
{
    return DereferenceHelper<const T>::pointer(this->ptr());
}

template <typename T>
bool Slot<T>::isCompatible(const AbstractSlot * source) const
{
    // Check if source is valid and compatible data container
    if (!source)
    {
        return false;
    }

    // Check if types are equal
    return this->type() == source->type();
}

template <typename T>
bool Slot<T>::connect(AbstractSlot * source)
{
    // Check if source is valid and compatible data container
    if (!source || !isCompatible(source))
    {
        return false;
    }

    // Connect to source data
    return connect(static_cast< Slot<T> * >(source));
}

template <typename T>
void Slot<T>::disconnect()
{
    // Reset source property
    m_source     = nullptr;
    m_connection = cppexpose::ScopedConnection();

    // Emit events
    this->promoteConnection();
    this->onValueChanged(this->m_value);
}

template <typename T>
const AbstractSlot * Slot<T>::source() const
{
    return m_source;
}

template <typename T>
bool Slot<T>::isValid() const
{
    // If connected, return validity of source slot
    if (m_source)
    {
        return m_source->isValid();
    }

    // Return validity of own data
    return m_valid;
}

template <typename T>
void Slot<T>::setValid(bool isValid)
{
    // If connected, abort function
    if (m_source)
    {
        return;
    }

    // Set state of own data
    m_valid = isValid;

    // Emit signal
    this->onValueChanged(this->m_value);
}

template <typename T>
void Slot<T>::onRequiredChanged()
{
    promoteRequired();
}

template <typename T>
T Slot<T>::value() const
{
    // If connected, return value of source slot
    if (m_source)
    {
        return m_source->value();
    }

    // Return own data
    return this->m_value;
}

template <typename T>
void Slot<T>::setValue(const T & value)
{
    // If connected, abort function
    if (m_source)
    {
        return;
    }

    // Set own data
    this->m_value = value;
    this->m_valid = true;

    // Emit signal
    this->onValueChanged(this->m_value);
}

template <typename T>
const T * Slot<T>::ptr() const
{
    // If connected, return value of source slot
    if (m_source)
    {
        return m_source->ptr();
    }

    // Return own data
    return &this->m_value;
}

template <typename T>
T * Slot<T>::ptr()
{
    // If connected, return value of source slot
    if (m_source)
    {
        return m_source->ptr();
    }

    // Return own data
    return &this->m_value;
}

template <typename T>
cppexpose::AbstractTyped * Slot<T>::clone() const
{
    return nullptr;
}

template <typename T>
bool Slot<T>::isObject() const
{
    return false;
}

template <typename T>
bool Slot<T>::isDynamic() const
{
    return m_dynamic;
}

template <typename T>
void Slot<T>::promoteConnection()
{
    // Emit signal
    this->connectionChanged();
    this->parentStage()->invalidateInputConnections();
}

template <typename T>
void Slot<T>::promoteRequired()
{
    // Check if input slot is connected
    if (!m_source)
    {
        return;
    }

    // Promote required-flag to connected slot
    m_source->setRequired(this->m_required);
}


} // namespace gloperate
