
#include <cassert>

#include <gloperate/input/InputManager.h>
#include <gloperate/input/AbstractEventConsumer.h>


namespace gloperate {

void InputManager::registerConsumer(AbstractEventConsumer * const consumer)
{
    assert(consumer != nullptr);
    m_consumers.push_back(consumer);
}

void InputManager::deregisterConsumer(AbstractEventConsumer * const consumer)
{
    assert(consumer != nullptr);
    m_consumers.remove(consumer);
}

void InputManager::onEvent(const InputEvent &event)
{
    for(auto consumer : m_consumers)
    {
        consumer->onEvent(event);
    }
}

} // namespace gloperate

