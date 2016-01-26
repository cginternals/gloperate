
#include <gloperate/input/WheelEvent.h>


namespace gloperate
{


WheelEvent::WheelEvent(
    const glm::ivec2 & pos, 
    const glm::vec2 & angleDelta, 
    const glm::ivec2 & pixelDelta, 
    MouseButton button)
    : m_pos(pos)
    , m_angleDelta(angleDelta)
    , m_pixelDelta(pixelDelta)
    , m_button(button)
{
    m_sourceType = EventType::Wheel;
}

MouseButton WheelEvent::button() const
{
    return m_button;
}
const glm::vec2 & WheelEvent::angleDelta() const
{
    return m_angleDelta;
}
const glm::ivec2 & WheelEvent::pixelDelta() const
{
    return m_pixelDelta;
}
const glm::ivec2 & WheelEvent::pos() const
{
    return m_pos;
}

int WheelEvent::x() const
{
    return m_pos.x;
}
int WheelEvent::y() const
{
    return m_pos.y;
}

float WheelEvent::defaultMouseAngleDelta()
{
    return 15.f;
}


} // namespace gloperate
