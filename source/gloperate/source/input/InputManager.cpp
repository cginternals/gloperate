
#include <cassert>

#include <gloperate/input/InputManager.h>

#include <algorithm>



namespace gloperate {

void InputManager::registerConsumer(AbstractEventConsumer * const consumer)
{
    assert(consumer != nullptr);
    m_consumers.push_back(consumer);
}

void InputManager::deregisterConsumer(AbstractEventConsumer * const consumer)
{
    assert(consumer != nullptr);
    auto consumerPos = std::find(m_consumers.begin(), m_consumers.end(), consumer);
    if(!consumerPos == m_consumers.end())
    {
        consumerPos;
    }
}

void InputManager::onEvent(const InputEvent &event)
{
    for(auto consumer : m_consumers)
    {
        consumer->onEvent(event);
    }
}
}
