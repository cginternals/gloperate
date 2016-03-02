
#include <cassert>

#include <gloperate/input/InputEvent.h>

namespace gloperate{

InputEvent::InputEvent(Type type, AbstractDevice * dispatchingDevice)
: m_type(type)
, m_dispatchingDevice(dispatchingDevice)
{
    assert(dispatchingDevice != nullptr);
}

InputEvent::~InputEvent()
{

}

InputEvent::Type InputEvent::type() const
{
    return m_type;
}

std::string InputEvent::asString() const
{
    return std::to_string(static_cast<int>(m_type));
}


MouseEvent::MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, int button)
: InputEvent(type, dispatchingDevice)
, m_pos(pos)
, m_button(button)
, m_wheelDelta(glm::vec2(.0f, .0f))
{
    assert(type == Type::MouseMove ||
           type == Type::MouseButtonPress ||
           type == Type::MouseButtonRelease);
}

MouseEvent::MouseEvent(InputEvent::Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::vec2 wheelDelta)
: InputEvent(type, dispatchingDevice)
, m_pos(pos)
, m_button(0)
, m_wheelDelta(wheelDelta)
{
    assert(type == Type::MouseWheelScroll);
}

int MouseEvent::x() const
{
    return m_pos.x;
}

int MouseEvent::y() const
{
    return m_pos.y;
}

int MouseEvent::button() const
{
    return m_button;
}

glm::vec2 MouseEvent::wheelDelta() const
{
    return m_wheelDelta;
}

std::string MouseEvent::asString() const
{
    return std::to_string(static_cast<int>(m_type));
}


ButtonEvent::ButtonEvent(Type type, AbstractDevice * dispatchingDevice, const std::string & description)
: InputEvent(type, dispatchingDevice)
, m_description(description)
{
    assert(type == Type::ButtonPress ||
           type == Type::ButtonRelease);

}

std::string ButtonEvent::asString() const
{
    return m_description;
}

} //namespace gloperate
