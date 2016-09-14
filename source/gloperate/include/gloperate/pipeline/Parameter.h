
#pragma once


#include <gloperate/pipeline/DataSlot.h>


namespace gloperate
{


/**
*  @brief
*    Parameter of a stage
*/
template <typename T>
class Parameter : public DataSlot<T>
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
    *    Creates the parameter and adds it to the given stage without ownership.
    *    Use this constructor for static slots, which are usually created
    *    as direct instances on a stage class.
    */
    Parameter(const std::string & name, Stage * parent, const T & defaultValue = T());

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
    *    Creates the parameter without adding it to any stage.
    *    Use this constructor for dynamic slots and call addInput()
    *    to add the slot to a stage.
    */
    Parameter(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Parameter();

    // Virtual AbstractDataSlot interface
    virtual void setValid(bool isValid) override;

    // Virtual AbstractSlot interface
    virtual bool isValid() const override;


protected:
    // Virtual AbstractSlot interface
    virtual void onRequiredChanged() override;

    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Parameter.inl>
