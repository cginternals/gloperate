
#pragma once


#include <string>
#include <memory>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class ChronoTimer;


/**
*  @brief
*    Basic, scoped performance measurements with minimal interface
*
*    AutoTimer can be used for quick performance measurements.
*    Just create an instance that, when deleted due to ending
*    scope, prints out the measured time. Example:
*    
*    \code{.cpp}
*    
*    {
*      AutoTimer t1("Initialization"); // starts timer
*      DoStuff();
*    } // stops timer and prints out the elapsed time.
*    
*    \endcode
*    
*    If more control over time measurement is required, consider 
*    using ChronoTimer directly.
*
*  @see ChronoTimer
*/
class GLOPERATE_API AutoTimer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] info
    *    Information that is printed with the debug message
    */
    AutoTimer(const std::string & info);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AutoTimer();


protected:
    static int s_numActiveInstances;

    std::string m_info;
    int m_index;

    std::unique_ptr<ChronoTimer> m_timer;
};


} // namespace gloperate
