
#pragma once

#include <vector>
#include <memory>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractDevice;

class InputManager
{
public:
	/**
	 * @brief
	 *	 Constructor
	 */
	GLOPERATE_API InputManager() = default;

	/**
	 * @brief
	 * 	Destructor
	 */
	GLOPERATE_API virtual ~InputManager() = default;

	/**
	 * @brief
	 *   Register a device
	 *
	 * @param[in] device
	 *   A pointer to the device being registered.
	 *   The device will be cleaned up by the Input Manager
	 *
	 */
	GLOPERATE_API void addDevice(AbstractDevice* device);

	/**
	 * @brief
	 *   Polls Inputs from all Devices
	 */
	GLOPERATE_API void update();


protected:
	std::vector<std::unique_ptr<AbstractDevice>> m_devices;
};

}
