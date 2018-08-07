
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

void MouseDevice::move(const glm::ivec2 & pos, int modifiers)
{
    auto inputEvent = cppassist::make_unique<MouseEvent>(
        InputEvent::Type::MouseMove,
        this,
        pos,
        modifiers
    );

    m_inputManager->onEvent(std::move(inputEvent));
}

void MouseDevice::buttonPress(int button, const glm::ivec2 & pos, int modifiers)
{
    auto inputEvent = cppassist::make_unique<MouseEvent>(
        InputEvent::Type::MouseButtonPress,
        this,
        pos,
        button,
        modifiers
    );

    m_inputManager->onEvent(std::move(inputEvent));
}

void MouseDevice::buttonRelease(int button, const glm::ivec2 & pos, int modifiers)
{
    auto inputEvent = cppassist::make_unique<MouseEvent>(
        InputEvent::Type::MouseButtonRelease,
        this,
        pos,
        button,
        modifiers
    );

    m_inputManager->onEvent(std::move(inputEvent));
}

void MouseDevice::wheelScroll(const glm::vec2 & delta, const glm::ivec2 & pos, int modifiers)
{
    auto inputEvent = cppassist::make_unique<MouseEvent>(
        InputEvent::Type::MouseWheelScroll,
        this,
        pos,
        delta,
        modifiers
    );

    m_inputManager->onEvent(std::move(inputEvent));
}

void MouseDevice::update()
{
}


} // namespace gloperate
