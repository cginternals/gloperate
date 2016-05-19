
#pragma once


#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/AbstractProxyOutput.h>


namespace gloperate
{


/**
*  @brief
*    Proxy output of a stage
*/
template <typename T>
class ProxyOutput : public InputSlot<T, AbstractProxyOutput>
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
    *    The proxy output is created and added to the given stage.
    */
    ProxyOutput(Stage * parent, const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ProxyOutput();
};


} // namespace cppexpose


#include <gloperate/pipeline/ProxyOutput.hpp>
