
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

public:
    GLOPERATE_HIDAPI_API HIDDeviceProvider(gloperate::InputManager* manager);
    GLOPERATE_HIDAPI_API virtual ~HIDDeviceProvider(){} ;

    GLOPERATE_HIDAPI_API virtual void updateDevices();

private:

    void createDevice(hid_device_info* device, const std::wstring &deviceID);

    std::unordered_map<std::wstring,device_ptr> m_openDevices;
};

}
