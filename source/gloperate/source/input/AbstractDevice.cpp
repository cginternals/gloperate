
#include <gloperate/input/AbstractDevice.h>

#include <cassert>

#include <gloperate/input/InputManager.h>


namespace gloperate
{


AbstractDevice::AbstractDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: m_inputManager(inputManager)
, m_deviceId(generateDeviceId())
, m_deviceDescriptor(deviceDescriptor)
{
    assert(m_inputManager != nullptr);

    m_inputManager->addDevice(this);
}

AbstractDevice::~AbstractDevice()
{
}

const std::string & AbstractDevice::deviceDescriptor() const
{
    return m_deviceDescriptor;
}

unsigned int AbstractDevice::generateDeviceId()
{
    maxDeviceId++;
    return maxDeviceId;
}

unsigned int AbstractDevice::maxDeviceId = 0;


} // namespace gloperate
