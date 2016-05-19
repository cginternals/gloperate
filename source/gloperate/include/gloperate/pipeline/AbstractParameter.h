
#pragma once


#include <gloperate/pipeline/AbstractData.h>


namespace gloperate
{


/**
*  @brief
*    Base class for stage parameters
*/
class GLOPERATE_API AbstractParameter : public AbstractData
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractParameter();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractParameter();


protected:
    /**
    *  @brief
    *    Initialize parameter
    *
    *  @param[in] owner
    *    Owner stage (can be null)
    */
    void initParameter(Stage * owner);
};


} // namespace cppexpose
