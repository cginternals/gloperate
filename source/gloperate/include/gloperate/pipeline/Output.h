
#pragma once


#include <gloperate/pipeline/Slot.h>


namespace gloperate
{


/**
*  @brief
*    Output data of a stage
*
*  @see AbstractSlot
*/
template <typename T>
class Output : public Slot<T>
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
    *    Creates the output slot and adds it to the given stage without ownership.
    *    Use this constructor for static slots, which are usually created as
    *    direct instances on a stage class.
    */
    Output(const std::string & name, Stage * parent, const T & defaultValue = T());

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
    *    Creates the output slot without adding it to any stage.
    *    Use this constructor for dynamic slots and call addOutput()
    *    to add the slot to a stage.
    */
    Output(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Output();

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
    Output<T> & operator=(const U & value);


protected:
    // Virtual AbstractSlot interface
    virtual void onValueInvalidated() override;
    virtual void onRequiredChanged() override;

    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Output.inl>
