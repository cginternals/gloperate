
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
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initParameter(Stage * parent, cppexpose::PropertyOwnership ownership);
};


} // namespace cppexpose
