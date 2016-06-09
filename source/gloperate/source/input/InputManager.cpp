
#include <gloperate/input/InputManager.h>

#include <cassert>

#include <gloperate/input/AbstractDeviceProvider.h>
#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/AbstractEventConsumer.h>
#include <gloperate/input/InputEvent.h>


namespace gloperate
{


InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	for (const auto ptr : m_consumers)
	{
		delete ptr;
	}

	for (const auto ptr : m_deviceProviders)
	{
		delete ptr;
	}

	for (const auto ptr : m_devices)
	{
		delete ptr;
	}

	for (const auto ptr : m_events)
	{
		delete ptr;
	}
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

void InputManager::addMapping(Mapping * mapping)
{
    m_mappings.push_back(mapping);
}

void InputManager::onEvent(InputEvent * event)
{
    assert(event != nullptr);
    m_events.emplace_back(event);

    for (auto consumer : m_consumers)
    {
        consumer->onEvent(event);
    }
}

void InputManager::onControlEvent(Control * control, InputEvent * event)
{
    for(auto& mapping : m_mappings)
    {
        mapping->onEvent(control->id(), event);
    }
}


} // namespace gloperate
