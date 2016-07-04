#pragma once


#include <gloperate/painter/AbstractCapability.h>
#include "AbstractInteraction.h"


namespace gloperate 
{
    enum class RoutingEventType
    {
        Any = -1,
        MouseClick = 0,
        MouseDrag,
        MouseWheel,
        Keyboard,
    };

    enum class RoutingEventValue
    {
        Any = -1
    };

/**
*  @brief
*    Capability that allows for specifying the output viewport of a painter
*
*    If a painter supports this capability, it can be used to specify the viewport
*    the painter uses when rendering an image.
*/
class GLOPERATE_API AbstractEventRoutingCapability : public AbstractCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractEventRoutingCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractEventRoutingCapability();

    virtual AbstractInteraction * mapEventToReciever(int id, RoutingEventType type, int eventValue) = 0;
    virtual AbstractInteraction * getNavigation() = 0;
};

} // namespace gloperate
