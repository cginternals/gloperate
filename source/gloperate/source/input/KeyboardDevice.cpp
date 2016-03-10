
#include <gloperate/input/KeyboardDevice.h>
#include <gloperate/input/ButtonEvent.h>
#include <gloperate/input/InputManager.h>

gloperate::KeyboardDevice::KeyboardDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: AbstractDevice(inputManager, deviceDescriptor)
{

}

gloperate::KeyboardDevice::~KeyboardDevice()
{

}

void gloperate::KeyboardDevice::update()
{

}

void gloperate::KeyboardDevice::keyPress(int key, int modifier)
{
    auto inputEvent = new gloperate::ButtonEvent{
        gloperate::InputEvent::Type::ButtonPress,
        this,
        std::to_string(key) + ":" + std::to_string(modifier)
    };

    m_inputManager->onEvent(inputEvent);

}

void gloperate::KeyboardDevice::keyRelease(int key, int modifier)
{
    auto inputEvent = new gloperate::ButtonEvent{
        gloperate::InputEvent::Type::ButtonPress,
        this,
        std::to_string(key) + ":" + std::to_string(modifier)
    };

    m_inputManager->onEvent(inputEvent);

}
