
#pragma once

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractDevice;
class InputManager;

class GLOPERATE_API AbstractDeviceProvider
{
public:
    /**
     * @brief
     *   Constructor
     */
    AbstractDeviceProvider() = default;

    /**
     * @brief
     *   Destructor
     */
    virtual ~AbstractDeviceProvider() = default;

    /**
     *  @brief
     *    Updates the device list and adds Devices to the Input Manager
     */
    virtual void updateDevices() = 0;

protected:
    InputManager * m_inputManager;

};

}
