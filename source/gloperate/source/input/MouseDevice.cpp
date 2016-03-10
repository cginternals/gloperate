
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/InputManager.h>

gloperate::MouseDevice::MouseDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: AbstractDevice(inputManager, deviceDescriptor)
{

}

gloperate::MouseDevice::~MouseDevice()
{

}

void gloperate::MouseDevice::update()
{

}

void gloperate::MouseDevice::move(const glm::ivec2 & pos)
{
    auto inputEvent = new gloperate::MouseEvent{
        gloperate::InputEvent::Type::MouseButtonPress,
        this,
        pos
    };

    m_inputManager->onEvent(inputEvent);
}

void gloperate::MouseDevice::buttonPress(int button, const glm::ivec2 & pos)
{
    auto inputEvent = new gloperate::MouseEvent{
        gloperate::InputEvent::Type::MouseButtonPress,
        this,
        pos,
        button
    };

    m_inputManager->onEvent(inputEvent);

}

void gloperate::MouseDevice::buttonRelease(int button, const glm::ivec2 & pos)
{
    auto inputEvent = new gloperate::MouseEvent{
        gloperate::InputEvent::Type::MouseButtonRelease,
        this,
        pos,
        button
    };

    m_inputManager->onEvent(inputEvent);

}

void gloperate::MouseDevice::wheelScroll(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    auto inputEvent = new gloperate::MouseEvent{
        gloperate::InputEvent::Type::MouseWheelScroll,
        this,
        pos,
        delta
    };

    m_inputManager->onEvent(inputEvent);

}
