
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/input/input.h>


namespace gloperate
{


/**
*  @brief
*    Input handler that receives keyboard events
*
*    This class can be used to register a handler that will receive direct keyboard
*    events from a window. It can be registered for example in a painter through
*    an InputCapability. The concrete viewer implementation is responsible for
*    querying the InputCapability on the current painter and forwarding all
*    events to the registered input handlers.
*
*    However, this functionality is mainly intended to be used for connecting gloperate
*    to other rendering systems, such as OSG, that will need these input events for their
*    interaction. When using gloperate itself, interaction should rather be implemented
*    by using the interaction abstraction interface, not by directly working with
*    mouse or keyboard events.
*/
class GLOPERATE_API KeyboardInputHandler
{
public:
    /**
    *  @brief
    *    Constructor
    */
    KeyboardInputHandler();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~KeyboardInputHandler();

    /**
    *  @brief
    *    Called on key down
    *
    *  @param[in] key
    *    Key
    */
    virtual void onKeyDown(gloperate::Key key);

    /**
    *  @brief
    *    Called on key up
    *
    *  @param[in] key
    *    Key
    */
    virtual void onKeyUp(gloperate::Key key);
};


} // namespace gloperate
