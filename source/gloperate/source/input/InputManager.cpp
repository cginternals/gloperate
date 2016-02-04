#include <gloperate/input/InputManager.h>

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/AbstractDeviceProvider.h>

#include <algorithm>

namespace gloperate
{

void InputManager::addDeviceProvider(AbstractDeviceProvider * provider)
{
    m_providers.emplace_back(provider);
}

void InputManager::update()
{
    for(auto& provider : m_providers)
    {
        provider->updateDevices();
    }
}

std::vector<std::weak_ptr<AbstractDevice>> InputManager::listDevices()
{
    std::vector<std::weak_ptr<AbstractDevice>> devices;
    for(auto& entry : m_devices)
    {
        devices.push_back(std::weak_ptr{entry.second});
    }
    return devices;
}

void InputManager::registerDevice(std::wstring deviceId, device_ptr device)
{
    if(m_devices.find(deviceID) == m_devices.end())
    {
        m_devices.emplace({deviceId, device});
    }
}

void InputManager::deregisterDevice(std::wstring deviceId)
{
    auto deviceIterator = m_devices.find(deviceId);
    if(deviceIterator == m_devices.end())
    {
        return;
    }
    m_devices.erase(deviceIterator);
}

}
