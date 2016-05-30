
#pragma once


#include <gloperate/pipeline/AbstractDataSlot.h>


namespace gloperate
{


/**
*  @brief
*    Base class for stage parameters
*/
class GLOPERATE_API AbstractParameter : public AbstractDataSlot
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
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initParameter(Stage * parent, cppexpose::PropertyOwnership ownership);
};


} // namespace cppexpose
