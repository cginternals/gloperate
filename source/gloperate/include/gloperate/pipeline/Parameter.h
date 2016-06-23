
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
    *    The parameter is created and added to the given stage.
    */
    Parameter(const std::string & name, Stage * parent, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Parameter();

    // Virtual AbstractSlot interface
    virtual bool isValid() const override;

protected:
    virtual void onRequiredChanged() override;
    virtual void onValueChanged(const T & value) override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Parameter.hpp>
