#include <gloperate/input/MouseEvent.h>

namespace gloperate
{

MouseEvent::MouseEvent(MouseEvent::Type type, const glm::ivec2 & pos, const glm::ivec2 & lastPos, const glm::ivec2 & screenSize, const MouseButton button, const MouseButton buttonMask, const KeyModifier modifiers)
: AbstractEvent()
, m_type(type)
, m_button(button)
, m_buttonMask(buttonMask)
, m_modifiers(modifiers)
, m_pos(pos)
, m_lastPos(lastPos)
, m_screenSize(screenSize)
{
    m_sourceType = SourceType::Mouse;
}

MouseEvent::Type MouseEvent::type() const
{
    return m_type;
}

MouseButton MouseEvent::button() const
{
    return m_button;
}

MouseButton MouseEvent::buttonMask() const
{
    return m_buttonMask;
}

KeyModifier MouseEvent::modifiers() const
{
    return m_modifiers;
}

glm::ivec2 MouseEvent::position() const
{
    return m_pos;
}

glm::ivec2 MouseEvent::lastPosition() const
{
    return m_lastPos;
}

glm::ivec2 MouseEvent::positionDelta() const
{
    return m_pos - m_lastPos;
}

glm::vec2 MouseEvent::normalizedPosition() const
{
    return normalize(m_pos);
}

glm::vec2 MouseEvent::normalizedLastPosition() const
{
    return normalize(m_lastPos);
}

glm::vec2 MouseEvent::normalizedPositionDelta() const
{
    return normalize(m_pos - m_lastPos);
}

glm::vec2 MouseEvent::normalize(const glm::ivec2 & pos) const
{
    if (m_screenSize == glm::vec2(0))
    {
        return glm::ivec2(0);
    }

    return glm::vec2(pos) / m_screenSize;
}

} // namespace gloperate
