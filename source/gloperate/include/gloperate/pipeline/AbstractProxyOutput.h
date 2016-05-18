
#pragma once


#include <gloperate/pipeline/AbstractInputSlot.h>


namespace gloperate
{


/**
*  @brief
*    Base class for stage outputs
*/
class GLOPERATE_API AbstractProxyOutput : public AbstractInputSlot
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractProxyOutput();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractProxyOutput();
};


} // namespace cppexpose
