
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

    /**
    *  @brief
    *    Invalidate output
    *
    *    This marks the output value as invalid.
    *    The state will remain until a new value is set.
    */
    virtual void invalidate() = 0;


protected:
    /**
    *  @brief
    *    Initialize output
    *
    *  @param[in] owner
    *    Owner stage (can be null)
    */
    void initOutput(Stage * owner);
};


} // namespace cppexpose
