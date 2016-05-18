
#pragma once


#include <gloperate/pipeline/AbstractData.h>


namespace gloperate
{


/**
*  @brief
*    Base class for stage outputs
*/
class GLOPERATE_API AbstractOutput : public AbstractData
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractOutput();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractOutput();
};


} // namespace cppexpose
