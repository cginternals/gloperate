
#pragma once

#include <gloperate/input/AbstractEventConsumer.h>

namespace gloperate{

AbstractEventConsumer::AbstractEventConsumer(InputManager *inputManager)
: m_inputManager(inputManager)
{
    m_inputManager->registerConsumer(this);
}

AbstractEventConsumer::~AbstractEventConsumer()
{
    m_inputManager->deregisterConsumer(this);
}

} //namespace gloperate
