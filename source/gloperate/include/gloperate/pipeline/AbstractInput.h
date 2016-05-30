
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
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initInput(Stage * parent, cppexpose::PropertyOwnership ownership);
};


} // namespace cppexpose
