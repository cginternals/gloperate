
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class InputManager;
class InputEvent;


/**
*  @brief:
*     Base class for input consumers
*
*     An input consumer is an object that reacts on input
*     events, such as a camera navigation or a manipulator
*     inside a 3D scene.
*
*     An input consumer can be instanciated anywhere, it will
*     register itself at the input manager and deregister itself
*     on destruction. Therefore, the input manager does not take
*     ownership over the input consumer.
*
*     The input consumer provides a list of interaction metaphors,
*     to which input events can be mapped. With this mapping,
*     the user can decide which action shall be triggered with
*     which input devices/controls. The implementation of metaphors
*     has to decide which type of events it supports, other events
*     will be ignored when mapped to the wrong type of metaphor.
*/
class GLOPERATE_API AbstractEventConsumer
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] inputManager
    *    Input manager (must NOT be null)
    */
    AbstractEventConsumer(InputManager * inputManager);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractEventConsumer();

    /**
    *  @brief
    *    Called when an input event has occurred
    *
    *  @param[in] event
    *    Input event (must NOT be null)
    */
    virtual void onEvent(InputEvent * event) = 0;


protected:
    InputManager * m_inputManager; ///< Input manager (must NOT be null!)
};


} // namespace gloperate
