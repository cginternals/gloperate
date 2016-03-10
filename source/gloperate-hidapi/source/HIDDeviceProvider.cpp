
#include <gloperate-hidapi/HIDDeviceProvider.h>

#include <gloperate/input/InputManager.h>
#include <gloperate-hidapi/SpaceNavigator.h>
#include <set>

#include <algorithm>

#include <iostream>

namespace gloperate_hidapi
{


HIDDeviceProvider::HIDDeviceProvider()
{

}

HIDDeviceProvider::~HIDDeviceProvider()
{

}

void HIDDeviceProvider::updateDevices()
{
    struct hid_device_info *devs, *curDev;

    devs = hid_enumerate(0x0, 0x0);
    curDev = devs;

    while (curDev) {
        std::wstring deviceID = std::wstring(L"HID")+std::wstring(curDev->product_string)+std::wstring(curDev->serial_number);

        if(m_openDevices.find(deviceID) == m_openDevices.end())
        {
            //Device does not exist, create it
            createDevice(curDev, deviceID);
        }
    }

    hid_free_enumeration(devs);

    for(auto& entry : m_openDevices)
    {
        entry.second->update();
    }
}


void HIDDeviceProvider::createDevice(hid_device_info* device, const std::wstring& deviceID)
{
    gloperate::AbstractDevice* handler = nullptr;
    switch(device->vendor_id)
    {
    case 1133:
        handler = new SpaceNavigator(m_inputManager, device->path, device->path);
        break;
    default:
        break;
    }
    if(handler != nullptr)
    {
        m_inputManager->addDevice(handler);
        m_openDevices.insert(std::make_pair(deviceID, handler));

        std::cout << "Device Added: ";
        std::wcout << deviceID << std::endl;
    }
}

} //namespace gloperate_hidapi
