
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/input/input.h>


namespace gloperate
{


/**
*  @brief
*    Input handler that receives mouse events
*
*    This class can be used to register a handler that will receive direct mouse
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
class GLOPERATE_API MouseInputHandler
{
public:
    /**
    *  @brief
    *    Constructor
    */
    MouseInputHandler();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MouseInputHandler();

    /**
    *  @brief
    *    Called on mouse move
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    */
    virtual void onMouseMove(int x, int y);

    /**
    *  @brief
    *    Called on mouse pressed
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    *  @param[in] button
    *    Mouse button
    */
    virtual void onMousePress(int x, int y, gloperate::MouseButton button);

    /**
    *  @brief
    *    Called on mouse released
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    *  @param[in] button
    *    Mouse button
    */
    virtual void onMouseRelease(int x, int y, gloperate::MouseButton button);

    /**
    *  @brief
    *    Called on mouse double-click
    *
    *  @param[in] x
    *    X position
    *  @param[in] y
    *    Y position
    *  @param[in] button
    *    Mouse button
    */
    virtual void onMouseDoubleClick(int x, int y, gloperate::MouseButton button);

    /**
    *  @brief
    *    Called on mouse wheel event
    *
    *  @param[in] dx
    *    X delta
    *  @param[in] y
    *    Y delta
    */
    virtual void onMouseWheel(int dx, int dy);
};


} // namespace gloperate
