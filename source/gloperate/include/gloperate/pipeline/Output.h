
#pragma once


#include <gloperate/pipeline/Data.h>
#include <gloperate/pipeline/AbstractOutput.h>


namespace gloperate
{


/**
*  @brief
*    Data output of a stage
*/
template <typename T>
class Output : public Data<T, AbstractOutput>
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
    *    The output is created and added to the given stage.
    */
    Output(Stage * parent, const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Output();
};


} // namespace cppexpose


#include <gloperate/pipeline/Output.hpp>
