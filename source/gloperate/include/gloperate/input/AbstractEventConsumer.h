
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


class InputManager;
class InputEvent;


/**
*  @brief:
*     A Consumer of Events
*/
class GLOPERATE_API AbstractEventConsumer
{
public:
    /**
     *  @brief
     *    Constructor
     *
     *  @param[in] inputManager
     *    A pointer to the inputManager (must NOT be null)
     */
    AbstractEventConsumer(InputManager * inputManager);

    /**
     *  @brief
     *    Destructor
     *
     *  @param[in] inputManager
     *    A pointer to the inputManager (must NOT be null)
     */
    virtual ~AbstractEventConsumer();

    /**
     *  @brief
     *    Constructor
     *
     *  @param[in] event
     *    A pointer to the Event (must NOT be null)
     */
    virtual void onEvent(InputEvent * event) = 0;


protected:
    InputManager * m_inputManager;
};


} //namespace gloperate
