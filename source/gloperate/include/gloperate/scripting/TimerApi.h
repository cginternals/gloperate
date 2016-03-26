
#pragma once


#include <reflectionzeug/Object.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Scripting API for accessing timer functions
*/
class GLOPERATE_API TimerApi : public reflectionzeug::Object
{
public:
    /**
    *  @brief
    *    Constructor
    */
    TimerApi();

    /**
    *  @brief
    *    Destructor
    */
    ~TimerApi();
};


} // namespace gloperate
