
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractDevice;
class AbstractDeviceProvider;

class InputManager
{
    using device_ptr = std::shared_ptr<gloperate::AbstractDevice>;
    using DeviceMap = std::unordered_map<std::wstring,device_ptr>;
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
     *   Register a device provider
     *
     * @param[in] provider
     *   A pointer to the provider being registered.
     *   The provider will be cleaned up by the Input Manager.
     */
    GLOPERATE_API void addDeviceProvider(AbstractDeviceProvider * provider);

    /**
	 * @brief
	 *   Polls Inputs from all Devices
	 */
    GLOPERATE_API void update();

    /**
     * @brief
     *   Lists all connected Devices
     * @return
     *   A vector containing all connected devices
     */
    GLOPERATE_API std::vector<std::weak_ptr<AbstractDevice>> listDevices();

    /**
     * @brief
     *   Registers a Device with the Input Manager
     */
    GLOPERATE_API void registerDevice(std::wstring deviceId, device_ptr device);

    /**
     * @brief
     *   Removes a Device from the Input Manager
     */
    GLOPERATE_API void deregisterDevice(std::wstring deviceId);


protected:
    std::vector<std::unique_ptr<AbstractDeviceProvider>> m_providers;
    DeviceMap m_devices;
};

}
