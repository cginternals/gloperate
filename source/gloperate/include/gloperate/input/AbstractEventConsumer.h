
#pragma once

#include <gloperate/input/InputEvent.h>
#include <gloperate/input/InputManager.h>

namespace gloperate
{

class InputManager;

/**
*  @brief:
*     A Consumer of Events
*/

class GLOPERATE_API AbstractEventConsumer
{

public:
    AbstractEventConsumer(InputManager * inputManager);

    virtual ~AbstractEventConsumer();

    virtual void onEvent(const InputEvent& event) = 0;

private:
    InputManager * m_inputManager;
};


} //namespace gloperate
