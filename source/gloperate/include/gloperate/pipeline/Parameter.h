
#pragma once


#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/AbstractParameter.h>


namespace gloperate
{


/**
*  @brief
*    Parameter of a stage
*/
template <typename T>
class Parameter : public Data<T, AbstractParameter>
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] name
    *    Property name
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The parameter is created and added to the given stage.
    */
    Parameter(Stage * parent, const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Parameter();

    // Virtual AbstractSlot interface
    virtual bool isValid() const override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Parameter.hpp>
