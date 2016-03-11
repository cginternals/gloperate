
#include <gloperate/input/KeyboardDevice.h>

#include <gloperate/input/ButtonEvent.h>
#include <gloperate/input/InputManager.h>


namespace gloperate
{


KeyboardDevice::KeyboardDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: AbstractDevice(inputManager, deviceDescriptor)
{
}

KeyboardDevice::~KeyboardDevice()
{
}

void KeyboardDevice::update()
{
}

void KeyboardDevice::keyPress(int key, int modifier)
{
    auto inputEvent = new ButtonEvent{
        InputEvent::Type::ButtonPress,
        this,
        std::to_string(key) + ":" + std::to_string(modifier)
    };

    m_inputManager->onEvent(inputEvent);
}

void KeyboardDevice::keyRelease(int key, int modifier)
{
    auto inputEvent = new ButtonEvent{
        InputEvent::Type::ButtonPress,
        this,
        std::to_string(key) + ":" + std::to_string(modifier)
    };

    m_inputManager->onEvent(inputEvent);
}


} // namespace gloperate
