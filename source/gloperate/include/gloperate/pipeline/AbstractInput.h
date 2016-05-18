
#pragma once


#include <gloperate/pipeline/AbstractInputSlot.h>


namespace gloperate
{


/**
*  @brief
*    Base class for stage inputs
*/
class GLOPERATE_API AbstractInput : public AbstractInputSlot
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractInput();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractInput();
};


} // namespace cppexpose
