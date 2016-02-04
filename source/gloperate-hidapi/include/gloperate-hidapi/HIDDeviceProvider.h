
#pragma once

#include <hidapi/hidapi.h>

#include <gloperate/input/AbstractDeviceProvider.h>

#include <gloperate-hidapi/gloperate-hidapi_api.h>

#include <memory>
#include <unordered_map>
#include <string>




namespace gloperate
{
    class InputManager;
    class AbstractDevice;
}

namespace gloperate_hidapi
{

class HIDDeviceProvider : public gloperate::AbstractDeviceProvider
{
    using device_ptr = std::shared_ptr<gloperate::AbstractDevice>;
    using DeviceMap = std::unordered_map<std::wstring,device_ptr>;

public:
    /**
     * @brief
     *   Constructor
     */
    GLOPERATE_HIDAPI_API HIDDeviceProvider();

    /**
     * @brief
     *   Destructor
     */
    GLOPERATE_HIDAPI_API virtual ~HIDDeviceProvider();

    /**
     * @brief
     *   Updates the list of active devices and polls from those still active
     */
    GLOPERATE_HIDAPI_API virtual void updateDevices() override;

    /**
     *  @brief
     *    Lists all active devices handled by this provider
     *  @return
     *    Returns a vector of all devices connected through this provider
     */
    GLOPERATE_HIDAPI_API virtual std::vector<device_ptr> listDevices() override;

private:

    void createDevice(hid_device_info * device, const std::wstring &deviceID);

    DeviceMap m_openDevices;
};

}
