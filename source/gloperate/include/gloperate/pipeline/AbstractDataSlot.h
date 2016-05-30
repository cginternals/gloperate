
#pragma once


#include <cppexpose/reflection/AbstractProperty.h>

#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Base class for data slots
*
*    Data slots are slots that contain actual data.
*    In the current pipeline architecture, there are two
*    types of data slots: outputs and parameters.
*/
class GLOPERATE_API AbstractDataSlot : public AbstractSlot
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Slot type
    */
    AbstractDataSlot(SlotType type);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDataSlot();
};


} // namespace cppexpose
