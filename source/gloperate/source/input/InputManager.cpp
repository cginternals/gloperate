#include <gloperate/input/InputManager.h>

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/AbstractDeviceProvider.h>

namespace gloperate
{

void InputManager::addDeviceProvider(AbstractDeviceProvider* provider)
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

}
