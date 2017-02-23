
#pragma once


#include <map>
#include <thread>
#include <mutex>

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
    };
    //@}


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
    Slot(SlotType slotType, const std::string & name, Stage * parent, const T & value = T(), bool dynamic = false);

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
    Slot(SlotType slotType, const std::string & name, const T & value = T(), bool dynamic = true);

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
    auto operator->() const -> typename DereferenceHelper<const T>::Pointer;
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

    bool isDynamic() const override;


protected:
    void promoteConnection();
    void promoteRequired();


protected:
    bool                        m_valid;      ///< Does the slot have a valid value?
    bool                        m_dynamic;     ///< Was the slot created dynamically or statically? (statically = registered during construction);
    Slot<T>                   * m_source;     ///< Connected slot (can be null)
    cppexpose::ScopedConnection m_connection; ///< Connection to changed-signal of source property

    std::map<std::thread::id, bool> m_cycleGuard; ///< Protection against cyclic propagation of change-events (one per thread to be thread-safe)
    std::recursive_mutex            m_cycleMutex; ///< Mutex for accessing the cycle guard map
};


} // namespace cppexpose


#include <gloperate/pipeline/Slot.inl>
