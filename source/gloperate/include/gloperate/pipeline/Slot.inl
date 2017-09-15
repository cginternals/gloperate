
#pragma once


#include <cppassist/logging/logging.h>

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
template <typename U>
auto Slot<T>::DereferenceHelper<U*>::pointer(U * const * value) -> Pointer
{
    return *value;
}


template <typename T>
Slot<T>::Slot(SlotType slotType, const std::string & name, Stage * parent, const T & value)
: AbstractSlot(name)
, m_value(value)
, m_valid(true)
, m_source(nullptr)
{
    // Do not add property to object, yet. Just initialize the property itself
    //this->initProperty(name, nullptr);

    // Initialize slot, will also add slot as a property
    this->initSlot(slotType, parent);
}

template <typename T>
Slot<T>::Slot(SlotType slotType, const std::string & name, const T & value)
: AbstractSlot(name)
, m_value(value)
, m_valid(true)
, m_source(nullptr)
{
    // Make as a dynamic slot
    this->m_dynamic = true;

    // TODO: reimplement
    // Do not add property to object, yet. Just initialize the property itself
    //this->initProperty(name, nullptr);

    // Initialize slot
    this->initSlot(slotType, nullptr);
}

template <typename T>
Slot<T>::~Slot()
{
}

template <typename T>
bool Slot<T>::connect(Slot<T> * source)
{
    cppassist::debug(2, "gloperate") << this->qualifiedName() << ": connect slot " << source->qualifiedName();

    // Check if source is valid
    if (!source) {
        return false;
    }

    // Set source
    m_source = source;

    // Connect to data container; no direct binding of member function to achive lookup
    m_valueConnection = m_source->valueChanged.connect([this] (const T & value)
    {
        this->onValueChanged(value);
    } );
    m_validConnection = m_source->valueInvalidated.connect([this] ()
    {
        this->onValueInvalidated();
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
auto Slot<T>::operator->() const -> const typename DereferenceHelper<T>::Pointer
{
    return DereferenceHelper<T>::pointer(this->ptr());
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
        cppassist::debug(2, "gloperate") << this->qualifiedName() << ": connect slot failed for " << source->qualifiedName();
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
    m_valueConnection = cppexpose::ScopedConnection();
    m_validConnection = cppexpose::ScopedConnection();

    cppassist::debug(2, "gloperate") << this->qualifiedName() << ": disconnect slot";

    // Emit events
    this->promoteConnection();
    this->onValueChanged(this->m_value.value());
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
void Slot<T>::invalidate()
{
    if (!m_valid)
    {
        return;
    }

    // If connected, abort function
    if (!m_source)
    {
        m_valid = false;
    }

    // Emit signal if it was invalidated
    this->onValueInvalidated();
}

template <typename T>
bool Slot<T>::hasChanged() const
{
    return m_changed;
}

template <typename T>
void Slot<T>::setChanged(bool hasChanged)
{
    m_changed = hasChanged;
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
    return this->m_value.value();
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
    this->onValueChanged(this->m_value.value());
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
    return this->m_value.ptr();
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
    return this->m_value.ptr();
}

template <typename T>
const cppexpose::Type & Slot<T>::type() const
{
    return m_value.type();
}

template <typename T>
cppexpose::Type & Slot<T>::type()
{
    return m_value.type();
}

template <typename T>
const cppexpose::AbstractBaseType * Slot<T>::baseType() const
{
    return m_value.baseType();
}

template <typename T>
cppexpose::AbstractBaseType * Slot<T>::baseType()
{
    return m_value.baseType();
}

template <typename T>
const cppexpose::Type & Slot<T>::elementType() const
{
    return m_value.elementType();
}

template <typename T>
cppexpose::Type & Slot<T>::elementType()
{
    return m_value.elementType();
}

template <typename T>
const std::string & Slot<T>::typeName() const
{
    static cppexpose::BaseType<T> subType;
    static const auto name = std::string("slot<") + subType.typeName() + ">";

    return name;
}

template <typename T>
bool Slot<T>::isNull() const
{
    return false;
}

template <typename T>
bool Slot<T>::isType() const
{
    return false;
}

template <typename T>
bool Slot<T>::isConst() const
{
    return m_source != nullptr;
}

template <typename T>
bool Slot<T>::isArray() const
{
    return false;
}

template <typename T>
bool Slot<T>::isDynamicArray() const
{
    return false;
}

template <typename T>
bool Slot<T>::isMap() const
{
    return false;
}

template <typename T>
bool Slot<T>::isBoolean() const
{
    return false;
}

template <typename T>
bool Slot<T>::isNumber() const
{
    return false;
}

template <typename T>
bool Slot<T>::isIntegral() const
{
    return false;
}

template <typename T>
bool Slot<T>::isUnsigned() const
{
    return false;
}

template <typename T>
bool Slot<T>::isFloatingPoint() const
{
    return false;
}

template <typename T>
bool Slot<T>::isString() const
{
    return false;
}

template <typename T>
std::unique_ptr<cppexpose::AbstractValueContainer> Slot<T>::createCopy() const
{
    return nullptr;
}

template <typename T>
bool Slot<T>::compareTypeAndValue(const AbstractValueContainer & value) const
{
    // TODO: implement
    return false;
}

template <typename T>
std::string Slot<T>::toString() const
{
    if (m_source)
    {
        return m_source->toString();
    }

    return m_value.toString();
}

template <typename T>
bool Slot<T>::fromString(const std::string & value)
{
    // TODO: implement
    return false;
}

template <typename T>
bool Slot<T>::toBool() const
{
    return false;
}

template <typename T>
bool Slot<T>::fromBool(bool value)
{
    return false;
}

template <typename T>
long long Slot<T>::toLongLong() const
{
    return 0;
}

template <typename T>
bool Slot<T>::fromLongLong(long long value)
{
    return false;
}

template <typename T>
unsigned long long Slot<T>::toULongLong() const
{
    return 0;
}

template <typename T>
bool Slot<T>::fromULongLong(unsigned long long value)
{
    return false;
}

template <typename T>
double Slot<T>::toDouble() const
{
    return 0.0;
}

template <typename T>
bool Slot<T>::fromDouble(double value)
{
    return false;
}

template <typename T>
cppexpose::Variant Slot<T>::toVariant() const
{
    if (m_source != nullptr)
    {
        return m_source->toVariant();
    }

    return m_value.toVariant();
}

template <typename T>
bool Slot<T>::fromVariant(const cppexpose::Variant & variant)
{
    return m_value.fromVariant(variant);

    // TODO: remove connection upon explicit value update?
}

template <typename T>
size_t Slot<T>::numElements() const
{
    return m_source != nullptr ? m_source->numElements() : m_value.numElements();
}

template <typename T>
cppexpose::Variant Slot<T>::element(size_t i) const
{
    return m_source != nullptr ? m_source->element(i) : m_value.element(i);
}

template <typename T>
void Slot<T>::setElement(size_t i, const cppexpose::Variant & value)
{
    if (isConst())
    {
        return;
    }

    m_value.setElement(i, value);
}

template <typename T>
void Slot<T>::pushElement(const cppexpose::Variant & value)
{
    if (isConst())
    {
        return;
    }

    m_value.pushElement(value);
}

template <typename T>
std::vector<std::string> Slot<T>::keys() const
{
    if (m_source != nullptr)
    {
        return m_source->keys();
    }

    return m_value.keys();
}

template <typename T>
cppexpose::Variant Slot<T>::element(const std::string & key) const
{
    if (m_source != nullptr)
    {
        return m_source->element(key);
    }

    return m_value.element(key);
}

template <typename T>
void Slot<T>::setElement(const std::string & key, const cppexpose::Variant & value)
{
    if (isConst())
    {
        return;
    }

    m_value.setElement(key, value);
}

template <typename T>
bool Slot<T>::isObject() const
{
    return false;
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
