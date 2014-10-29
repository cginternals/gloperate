#include <gloperate/input/MouseEvent.h>

namespace gloperate
{

MouseEvent::MouseEvent(EventType eventType, const glm::ivec2 & pos, const MouseButton button, const int modifiers)
: AbstractEvent(eventType)
, m_button(button)
, m_modifiers(modifiers)
, m_pos(pos)
{
    m_sourceType = SourceType::Mouse;
}

MouseButton MouseEvent::button() const
{
    return m_button;
}

int MouseEvent::modifiers() const
{
    return m_modifiers;
}

int MouseEvent::x() const
{
    return m_pos.x;
}

int MouseEvent::y() const
{
    return m_pos.y;
}

const glm::ivec2 & MouseEvent::pos() const
{
    return m_pos;
}

} // namespace gloperate
