
#include <gloperate/input/AxisEvent.h>


namespace gloperate
{


AxisEvent::AxisEvent(InputEvent::Type type, AbstractDevice * dispatchingDevice, glm::mat3 value, glm::mat3 delta)
: InputEvent(type, dispatchingDevice)
, m_value(value)
, m_delta(delta)
{
    assert(type == Type::SpatialAxis);
}

const glm::mat3 & AxisEvent::value() const
{
    return m_value;
}

const glm::mat3 &AxisEvent::delta() const
{
    return m_delta;
}

std::string AxisEvent::asString() const
{
    return std::to_string(static_cast<int>(m_type));
}


} // namespace gloperate
