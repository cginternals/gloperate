
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

void KeyboardDevice::keyPress(int key, int modifier)
{
    auto inputEvent = cppassist::make_unique<ButtonEvent>(
        InputEvent::Type::ButtonPress,
        this,
        key,
        modifier
    );

    m_inputManager->onEvent(std::move(inputEvent));
}

void KeyboardDevice::keyRelease(int key, int modifier)
{
    auto inputEvent = cppassist::make_unique<ButtonEvent>(
        InputEvent::Type::ButtonRelease,
        this,
        key,
        modifier
    );

    m_inputManager->onEvent(std::move(inputEvent));
}

void KeyboardDevice::update()
{
}


} // namespace gloperate
