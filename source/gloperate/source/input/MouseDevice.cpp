
#include <gloperate/input/MouseDevice.h>

#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/InputManager.h>


namespace gloperate
{


MouseDevice::MouseDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: AbstractDevice(inputManager, deviceDescriptor)
, m_moveControl(m_deviceId, 0, inputManager)
, m_clickControl(m_deviceId, 1, inputManager)
{
}

MouseDevice::~MouseDevice()
{
}

void MouseDevice::move(const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseButtonPress,
        this,
        pos,
        pos-m_lastPos
    };

    m_inputManager->onEvent(inputEvent);

    m_moveControl.dispatch(inputEvent);

    m_lastPos = pos;
}

void MouseDevice::buttonPress(int button, const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseButtonPress,
        this,
        pos,
        pos-m_lastPos,
        button
    };

    m_inputManager->onEvent(inputEvent);

    m_clickControl.dispatch(inputEvent);

    m_lastPos = pos;
}

void MouseDevice::buttonRelease(int button, const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseButtonRelease,
        this,
        pos,
        pos-m_lastPos,
        button
    };

    m_inputManager->onEvent(inputEvent);

    m_lastPos = pos;
}

void MouseDevice::wheelScroll(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseWheelScroll,
        this,
        pos,
        pos-m_lastPos,
        delta
    };

    m_inputManager->onEvent(inputEvent);

    m_lastPos = pos;
}

void MouseDevice::update()
{
}


} // namespace gloperate
