
#pragma once


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
    *  @param[in] value
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
    *  @param[in] value
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


protected:
    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Input.inl>
