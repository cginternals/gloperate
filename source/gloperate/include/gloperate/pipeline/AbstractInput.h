
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


protected:
    /**
    *  @brief
    *    Initialize input
    *
    *  @param[in] owner
    *    Owner stage (can be null)
    */
    void initInput(Stage * owner);
};


} // namespace cppexpose
