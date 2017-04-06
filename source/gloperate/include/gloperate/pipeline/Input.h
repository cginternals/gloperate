
#pragma once


#include <map>
#include <thread>
#include <mutex>

#include <gloperate/pipeline/Slot.h>


namespace gloperate
{


/**
*  @brief
*    Input data of a stage
*
*  @see AbstractSlot
*/
template <typename T>
class Input : public Slot<T>
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Property name
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] defaultValue
    *    Default value
    *
    *  @remarks
    *    Creates the input slot and adds it to the given stage without ownership.
    *    Use this constructor for static slots, which are usually created as
    *    direct instances on a stage class.
    */
    Input(const std::string & name, Stage * parent, const T & defaultValue = T());

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Property name
    *  @param[in] defaultValue
    *    Default value
    *
    *  @remarks
    *    Creates the input slot without adding it to any stage.
    *    Use this constructor for dynamic slots and call addInput()
    *    to add the slot to a stage.
    */
    Input(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Input();

    /**
    *  @brief
    *    Set value
    *
    *  @tparam[in] U
    *    Type of the assigned value. Must be implicitly convertible to T.
    *  @param[in] value
    *    Value
    *    
    *  @remarks
    *    Has the same effect as calling setValue().
    *    
    *  @see setValue()
    */
    template <typename U, typename Enable = typename std::enable_if<std::is_convertible<U, T>::value>::type>
    Input<T> & operator=(const U & value);


protected:
    // Virtual AbstractSlot interface
    virtual void onValueInvalidated() override;

    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;

protected:
    // Data
    std::map<std::thread::id, bool> m_cycleGuard; ///< Protection against cyclic propagation of change-events (one per thread to be thread-safe)
    std::recursive_mutex            m_cycleMutex; ///< Mutex for accessing the cycle guard map
};


} // namespace cppexpose


#include <gloperate/pipeline/Input.inl>
