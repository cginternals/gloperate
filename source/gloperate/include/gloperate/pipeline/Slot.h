
#pragma once


#include <cppexpose/signal/Signal.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/gloperate_api.h>
#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


/**
*  @brief
*    Data slot on a stage
*
*  @see AbstractSlot
*/
template <typename T>
class GLOPERATE_TEMPLATE_API Slot : public AbstractSlot
{
protected:
    //@{
    /**
    * @see operator->()
    */
    template <typename U>
    struct DereferenceHelper
    {
        using Pointer = U *;
        static Pointer pointer(U * value);
    };

    template <typename U>
    struct DereferenceHelper<U *>
    {
        using Pointer = U *;
        static Pointer pointer(U ** value);
        static Pointer pointer(U * const * value);
    };
    //@}


public:
    cppexpose::Signal<const T &> valueChanged;      ///< Called when the value has been changed
    cppexpose::Signal<>          valueInvalidated;  ///< Called when the value was invalidated but not changed
    cppexpose::Signal<>          connectionChanged; ///< Called when the connection has been changed


public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] slotType
    *    Type or role of the slot (input or output)
    *  @param[in] name
    *    Property name
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The slot is created and added to the given stage
    *    without transferring ownership. This will set
    *    isDynamic to false.
    */
    Slot(SlotType slotType, const std::string & name, Stage * parent, const T & value = T());

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] slotType
    *    Type or role of the slot (input or output)
    *  @param[in] name
    *    Property name
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The slot is created but not added to any stage.
    *    This will set isDynamic to true.
    */
    Slot(SlotType slotType, const std::string & name, const T & value = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Slot();

    /**
    *  @brief
    *    Connect slot
    *
    *  @param[in] source
    *    Source slot (can be null)
    *
    *  @return
    *    'true' if slot could be connected, else 'false'
    */
    bool connect(Slot<T> * source);

    /**
    *  @brief
    *    Connect slot
    *
    *  @param[in] source
    *    Source slot
    *
    *  @return
    *    Reference to this slot
    */
    Slot<T> & operator<<(Slot<T> & source);

    /**
    *  @brief
    *    Dereference pointer operator
    *
    *  @return
    *    Reference to the stored data object
    */
    const T & operator*() const;
    //@}

    //@{
    /**
    *  @brief
    *    Dereference pointer operator
    *
    *  @return
    *    Pointer to the stored data object (non-pointer T) or the stored pointer (pointer T)
    */
    auto operator->() -> typename DereferenceHelper<T>::Pointer;
    auto operator->() const -> const typename DereferenceHelper<T>::Pointer;
    //@}

    // Virtual AbstractSlot interface
    virtual bool isCompatible(const AbstractSlot * source) const override;
    virtual bool connect(AbstractSlot * source) override;
    virtual void disconnect() override;
    virtual const AbstractSlot * source() const override;
    virtual bool isValid() const override;
    virtual void invalidate() override;
    virtual bool hasChanged() const override;
    virtual void setChanged(bool hasChanged) override;
    virtual void onRequiredChanged() override;

    // Typed value interface
    virtual T value() const;
    virtual void setValue(const T & value);
    virtual const T * ptr() const;
    virtual T * ptr();

    // Virtual Typed interface
    virtual const cppexpose::Type & type() const override;
    virtual cppexpose::Type & type() override;
    virtual const cppexpose::AbstractBaseType * baseType() const override;
    virtual cppexpose::AbstractBaseType * baseType() override;
    virtual const cppexpose::Type & elementType() const override;
    virtual cppexpose::Type & elementType() override;
    virtual const std::string & typeName() const override;
    virtual bool isNull() const override;
    virtual bool isType() const override;
    virtual bool isConst() const override;
    virtual bool isArray() const override;
    virtual bool isDynamicArray() const override;
    virtual bool isMap() const override;
    virtual bool isBoolean() const override;
    virtual bool isNumber() const override;
    virtual bool isIntegral() const override;
    virtual bool isUnsigned() const override;
    virtual bool isFloatingPoint() const override;
    virtual bool isString() const override;

    // Virtual AbstractValueContainer interface
    virtual std::unique_ptr<AbstractValueContainer> createCopy() const override;
    virtual bool compareTypeAndValue(const AbstractValueContainer & value) const override;
    virtual std::string toString() const override;
    virtual bool fromString(const std::string & value) override;
    virtual bool toBool() const override;
    virtual bool fromBool(bool value) override;
    virtual long long toLongLong() const override;
    virtual bool fromLongLong(long long value) override;
    virtual unsigned long long toULongLong() const override;
    virtual bool fromULongLong(unsigned long long value) override;
    virtual double toDouble() const override;
    virtual bool fromDouble(double value) override;
    virtual size_t numElements() const override;
    virtual cppexpose::Variant element(size_t i) const override;
    virtual void setElement(size_t i, const cppexpose::Variant & value) override;
    virtual void pushElement(const cppexpose::Variant & value) override;
    virtual std::vector<std::string> keys() const override;
    virtual cppexpose::Variant element(const std::string & key) const override;
    virtual void setElement(const std::string & key, const cppexpose::Variant & value) override;

    // Virtual AbstractProperty interface
    virtual bool isObject() const override;

    // Required interface from Input<T> and Output<T>
    virtual void onValueChanged(const T & value) = 0;


protected:
    void promoteConnection();
    void promoteRequired();


protected:
    cppexpose::InternalValue<T> m_value;      ///< Actual value
    bool                        m_valid;      ///< Does the slot have a valid value?
    bool                        m_changed;    ///< Was the slot changed since the last time it's pipeline was processed
    Slot<T>                   * m_source;     ///< Connected slot (can be null)
    cppexpose::ScopedConnection m_valueConnection; ///< Connection to changed-signal of source slot; removes the connection when destroyed
    cppexpose::ScopedConnection m_validConnection; ///< Connection to invalidated-signal of source slot; removes the connection when destroyed
};


} // namespace gloperate


#include <gloperate/pipeline/Slot.inl>
