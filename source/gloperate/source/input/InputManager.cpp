
#include <cassert>

#include <gloperate/input/InputManager.h>
#include <gloperate/input/AbstractEventConsumer.h>


namespace gloperate {

void InputManager::registerConsumer(AbstractEventConsumer * consumer)
{
    assert(consumer != nullptr);
    m_consumers.push_back(consumer);
}

void InputManager::deregisterConsumer(AbstractEventConsumer * consumer)
{
    assert(consumer != nullptr);
    m_consumers.remove(consumer);
}

void InputManager::onEvent(InputEvent * event)
{
    m_events.emplace_back(std::unique_ptr<InputEvent>(event));
    for(auto consumer : m_consumers)
    {
        consumer->onEvent(event);
    }
}

} // namespace gloperate

