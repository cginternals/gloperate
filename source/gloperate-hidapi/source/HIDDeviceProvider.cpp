

#include <gloperate-hidapi/HIDDeviceProvider.h>

#include <gloperate/input/InputManager.h>
#include <gloperate-hidapi/SpaceNavigator.h>
#include <set>

#include <iostream>

namespace gloperate_hidapi
{

HIDDeviceProvider::HIDDeviceProvider(gloperate::InputManager* manager)
    : gloperate::AbstractDeviceProvider(manager)
{

}

void HIDDeviceProvider::updateDevices()
{
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    std::set<std::wstring> foundDevices;
    while (cur_dev) {
        std::wstring deviceID = std::wstring(L"HID")+std::wstring(cur_dev->product_string)+std::wstring(cur_dev->serial_number);
        foundDevices.insert(deviceID);

        if(m_openDevices.find(deviceID) == m_openDevices.end())
        {

            createDevice(cur_dev, deviceID);

        }

        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    for(auto& entry : m_openDevices)
    {
        if(foundDevices.find(entry.first) != foundDevices.end())
        {
            m_openDevices.erase(entry.first);
        }
        else
        {
            entry.second->update();
        }
    }
}

void HIDDeviceProvider::createDevice(hid_device_info* device, const std::wstring& deviceID)
{
    gloperate::AbstractDevice* handler = nullptr;
    switch(device->vendor_id)
    {
    case 1133:
        handler = new SpaceNavigator(device->path);
        break;
    default:
        break;
    }
    if(handler != nullptr)
    {
        m_openDevices.insert(std::make_pair(deviceID, device_ptr(handler)));
            std::cout << "device added" << std::endl;
    }
}

}
