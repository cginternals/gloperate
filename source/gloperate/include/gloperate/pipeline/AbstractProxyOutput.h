
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


protected:
    /**
    *  @brief
    *    Initialize proxy output
    *
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initProxyOutput(Stage * parent, cppexpose::PropertyOwnership ownership);
};


} // namespace cppexpose
