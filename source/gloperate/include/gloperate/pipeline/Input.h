
#pragma once


#include <gloperate/pipeline/InputSlot.h>


namespace gloperate
{


/**
*  @brief
*    Data input of a stage
*/
template <typename T>
class Input : public InputSlot<T>
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
    *    The input is created and added to the given stage.
    */
    Input(const std::string & name, Stage * parent, const T & defaultValue = T());

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
