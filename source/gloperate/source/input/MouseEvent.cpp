
#include <gloperate/input/MouseEvent.h>

#include <glm/ext.hpp>


namespace gloperate
{


MouseEvent::MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::ivec2 deltaPos, int button)
: InputEvent(type, dispatchingDevice)
, m_pos(pos)
, m_deltaPos(deltaPos)
, m_button(button)
, m_wheelDelta(glm::vec2(.0f, .0f))
{
    assert(type == Type::MouseMove ||
           type == Type::MouseButtonPress ||
           type == Type::MouseButtonRelease);
}

MouseEvent::MouseEvent(InputEvent::Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::ivec2 deltaPos, glm::vec2 wheelDelta)
: InputEvent(type, dispatchingDevice)
, m_pos(pos)
, m_deltaPos(deltaPos)
, m_button(0)
, m_wheelDelta(wheelDelta)
{
    assert(type == Type::MouseWheelScroll);
}

glm::ivec2 MouseEvent::pos() const
{
    return m_pos;
}

glm::ivec2 MouseEvent::deltaPos() const
{
    return m_deltaPos;
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
    return typeToString(m_type) + " " + glm::to_string(m_pos);
}


} // namespace gloperate
