
#include <gloperate/input/InputManager.h>

#include <cassert>

#include <gloperate/input/AbstractDeviceProvider.h>
#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/AbstractEventConsumer.h>
#include <gloperate/input/InputEvent.h>
#include <gloperate/input/ButtonEvent.h>
#include <gloperate/input/MouseEvent.h>


namespace gloperate
{


InputManager::InputManager(Environment * environment)
: cppexpose::Object("input")
, m_environment(environment)
, m_nextId(1)
{
    // Register functions
    addFunction("onInput", this, &InputManager::scr_onInput);
}

InputManager::~InputManager()
{
}

void InputManager::registerConsumer(AbstractEventConsumer * consumer)
{
    assert(consumer != nullptr);
    m_consumers.push_back(consumer);
}

void InputManager::deregisterConsumer(AbstractEventConsumer * consumer)
{
    assert(consumer != nullptr);
    m_consumers.remove(consumer);
}

void InputManager::addDevice(AbstractDevice * device)
{
    assert(device != nullptr);
    m_devices.emplace_back(device);
}

void InputManager::onEvent(std::unique_ptr<InputEvent> && event)
{
    assert(event != nullptr);

    for (auto consumer : m_consumers)
    {
        consumer->onEvent(event.get());
    }

    for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
    {
        cppexpose::Function & func = it->second;
        std::string device   = "";
        std::string type     = "";
        int         key      = 0;
        int         modifier = 0;
        int         button   = 0;
        int         x        = 0;
        int         y        = 0;
        int         wx       = 0;
        int         wy       = 0;

        device = event->device()->deviceDescriptor();

        if (event->type() == InputEvent::Type::ButtonPress || event->type() == InputEvent::Type::ButtonRelease)
        {
            auto buttonEvent = static_cast<ButtonEvent *>(event.get());

                 if (event->type() == InputEvent::Type::ButtonPress)   type = "ButtonPress";
            else if (event->type() == InputEvent::Type::ButtonRelease) type = "ButtonRelease";
            key      = buttonEvent->key();
            modifier = buttonEvent->modifier();
            button   = 0;
            x        = 0;
            y        = 0;
            wx       = 0;
            wy       = 0;
        }

        else if (event->type() == InputEvent::Type::MouseMove || event->type() == InputEvent::Type::MouseButtonPress || event->type() == InputEvent::Type::MouseButtonRelease)
        {
            auto mouseEvent = static_cast<MouseEvent *>(event.get());

                 if (event->type() == InputEvent::Type::MouseMove)          type = "MouseMove";
            else if (event->type() == InputEvent::Type::MouseButtonPress)   type = "MouseButtonPress";
            else if (event->type() == InputEvent::Type::MouseButtonRelease) type = "MouseButtonRelease";
            key      = 0;
            modifier = 0;
            button   = mouseEvent->button();
            x        = mouseEvent->pos().x;
            y        = mouseEvent->pos().y;
            wx       = mouseEvent->wheelDelta().x;
            wy       = mouseEvent->wheelDelta().y;
        }

        func.call({
            device,
            type,
            key,
            modifier,
            button,
            x,
            y,
            wx,
            wy
        });
    }

	m_events.push_back(std::move(event));
}

int InputManager::scr_onInput(const cppexpose::Variant & func)
{
    // Check if a function has been passed
    if (!func.hasType<cppexpose::Function>())
    {
        return -1;
    }

    // Get callback function
    cppexpose::Function function = func.value<cppexpose::Function>();

    // Store timer
    int id = m_nextId++;
    m_callbacks[id] = std::move(function);

    // Return timer ID
    return id;
}


} // namespace gloperate
