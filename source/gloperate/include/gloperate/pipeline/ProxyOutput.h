
#pragma once


#include <gloperate/pipeline/InputSlot.h>


namespace gloperate
{


/**
*  @brief
*    Proxy output of a stage
*/
template <typename T>
class ProxyOutput : public InputSlot<T>
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
    *    Creates the proxy output and adds it to the given stage without ownership.
    *    Use this constructor for static slots, which are usually created
    *    as direct instances on a stage class.
    */
    ProxyOutput(const std::string & name, Stage * parent, const T & defaultValue = T());

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
    *    Creates the proxy output without adding it to any stage.
    *    Use this constructor for dynamic slots and call addInput()
    *    to add the slot to a stage.
    */
    ProxyOutput(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ProxyOutput();


protected:
    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;
};


} // namespace cppexpose


#include <gloperate/pipeline/ProxyOutput.inl>
