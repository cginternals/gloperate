
#pragma once

#include <gloperate/input/InputEvent.h>

namespace gloperate
{

/**
*  @brief:
*     A Consumer of Events
*/

class GLOPERATE_API AbstractEventConsumer
{

public:
    AbstractEventConsumer() = delete;

    virtual ~AbstractEventConsumer() = delete;

    void onEvent(const InputEvent& event) = 0;
};


} //namespace gloperate
