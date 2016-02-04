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

std::vector<std::shared_ptr<AbstractDevice>> InputManager::listDevices()
{

    std::vector<std::shared_ptr<AbstractDevice>> devices;
    /*for(auto& provider : m_providers)
    {
        auto providedDevices = provider->listDevices();
        std::move(providedDevices.begin(), providedDevices.end(), std::back_inserter(devices));
    }*/
    return devices;
}

}
