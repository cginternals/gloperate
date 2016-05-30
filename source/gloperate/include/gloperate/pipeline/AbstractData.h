
#pragma once


#include <cppexpose/reflection/AbstractProperty.h>

#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Base class for data containers
*/
class GLOPERATE_API AbstractData : public AbstractSlot
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Slot type
    */
    AbstractData(SlotType type);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractData();
};


} // namespace cppexpose
