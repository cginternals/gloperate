
#include <gloperate/input/MouseDevice.h>

#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/InputManager.h>


namespace gloperate
{


MouseDevice::MouseDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: AbstractDevice(inputManager, deviceDescriptor)
{
}

MouseDevice::~MouseDevice()
{
}

void MouseDevice::update()
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


} // namespace gloperate
