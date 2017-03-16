
#include <gloperate/input/InputManager.h>

#include <cassert>

#include <gloperate/input/AbstractDeviceProvider.h>
#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/AbstractEventConsumer.h>
#include <gloperate/input/InputEvent.h>


namespace gloperate
{


InputManager::InputManager(Environment * environment)
: cppexpose::Object("input")
, m_environment(environment)
{
}

InputManager::~InputManager()
{
}

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
    m_devices.emplace_back(device);
}

void InputManager::onEvent(std::unique_ptr<InputEvent> && event)
{
    assert(event != nullptr);

    for (auto consumer : m_consumers)
    {
        consumer->onEvent(event.get());
    }

	m_events.push_back(std::move(event));
}


} // namespace gloperate
