
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
    AbstractEventConsumer(InputManager * inputManager);

    virtual ~AbstractEventConsumer();

    virtual void onEvent(InputEvent * event) = 0;

protected:
    InputManager * m_inputManager;
};


} //namespace gloperate
