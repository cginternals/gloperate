#include <cassert>

#include <gloperate/input/AbstractDevice.h>

#include <gloperate/input/InputManager.h>

namespace gloperate{

AbstractDevice::AbstractDevice(InputManager *inputManager)
: m_inputManager{inputManager}
{
    assert(m_inputManager != nullptr);
}

AbstractDevice::~AbstractDevice()
{

}

} // namespace gloperate
