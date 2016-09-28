
#pragma once


#include <cppexpose/typed/DirectValue.h>
#include <cppexpose/signal/Signal.h>
#include <cppexpose/signal/ScopedConnection.h>

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
class Slot : public cppexpose::DirectValue<T, AbstractSlot>
{
public:
    cppexpose::Signal<const T &> valueChanged;      ///< Called when the value has been changed
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
    *    The input slot is created and added to the given stage
    *    without transferring ownership.
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
    *    The input slot is created but not added to any stage.
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
    //@}

    //@{
    /**
    *  @brief
    *    Dereference pointer operator
    *
    *  @return
    *    Reference to the stored data object
    */
    T & operator*();
    const T & operator*() const;
    //@}

    //@{
    /**
    *  @brief
    *    Dereference pointer operator
    *
    *  @return
    *    Pointer to the stored data object (never null)
    */
    T * operator->();
    const T * operator->() const;
    //@}

    // Virtual AbstractSlot interface
    virtual bool isCompatible(const AbstractSlot * source) const override;
    virtual bool connect(AbstractSlot * source) override;
    virtual void disconnect() override;
    virtual const AbstractSlot * source() const override;
    virtual bool isValid() const override;
    virtual void setValid(bool isValid) override;
    virtual void onRequiredChanged() override;

    // Virtual Typed<T> interface
    virtual T value() const override;
    virtual void setValue(const T & value) override;
    virtual const T * ptr() const override;
    virtual T * ptr() override;

    // Virtual AbstractTyped interface
    virtual cppexpose::AbstractTyped * clone() const override;

    // Virtual AbstractProperty interface
    virtual bool isObject() const override;


protected:
    void promoteConnection();
    void promoteRequired();


protected:
    bool                        m_valid;        ///< Does the slot have a valid value?
    Slot<T>                   * m_source;       ///< Connected slot (can be null)
    cppexpose::ScopedConnection m_connection;   ///< Connection to changed-signal of source property
};


} // namespace cppexpose


#include <gloperate/pipeline/Slot.inl>
