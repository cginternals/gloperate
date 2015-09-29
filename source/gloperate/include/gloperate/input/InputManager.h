
#pragma once

#include <string>
#include <vector>
#include <memory>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractDevice;
class AbstractDeviceProvider;

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
     *   Register a device provider
     *
     * @param[in] provider
     *   A pointer to the provider being registered.
     *   The provider will be cleaned up by the Input Manager.
     */
    GLOPERATE_API void addDeviceProvider(AbstractDeviceProvider* provider);

    /**
	 * @brief
	 *   Polls Inputs from all Devices
	 */
	GLOPERATE_API void update();


protected:
    std::vector<std::unique_ptr<AbstractDeviceProvider>> m_providers;
};

}
