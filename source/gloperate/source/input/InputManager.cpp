
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

void InputManager::addDevice(AbstractDevice * device)
{
    assert(device != nullptr);
    m_devices.emplace_back(std::unique_ptr<AbstractDevice>(device));
}

void InputManager::onEvent(InputEvent * event)
{
    assert(event != nullptr);
    m_events.emplace_back(std::unique_ptr<InputEvent>(event));
    for(auto consumer : m_consumers)
    {
        consumer->onEvent(event);
    }
}

} // namespace gloperate

