
#pragma once


#include <reflectionzeug/Object.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Test scripting API
*/
class GLOPERATE_API TestApi : public reflectionzeug::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Object name
    *  @param[in] value
    *    Integer value
    */
    TestApi(const std::string & name, int value);

    /**
    *  @brief
    *    Destructor
    */
    ~TestApi();

    /**
    *  @brief
    *    Print test message to the console
    *
    *  @param[in] msg
    *    Message
    *  @param[in] count
    *    Number of times
    */
    void sayHello(const std::string & msg, int count);


protected:
    /**
    *  @brief
    *    Get value
    *
    *  @return
    *    Integer value
    */
    int value() const;

    /**
    *  @brief
    *    Set value
    *
    *  @param[in] value
    *    Integer value
    */
    void setValue(int value);


protected:
    int m_value; ///< Integer value
};


} // namespace gloperate
