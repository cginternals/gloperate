
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
