#include <gloperate/input/InputManager.h>

#include <gloperate/input/AbstractDevice.h>

namespace gloperate
{

    void InputManager::addDevice(AbstractDevice* device)
	{
		m_devices.emplace_back(device);
	}
 
	void InputManager::update()
	{
		for(auto& device : m_devices)
		{
			device->update();
		}
	}

}
