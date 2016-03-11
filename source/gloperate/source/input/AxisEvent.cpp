
#include <gloperate/input/AxisEvent.h>


namespace gloperate
{


AxisEvent::AxisEvent(InputEvent::Type type, AbstractDevice * dispatchingDevice, glm::mat3 value)
: InputEvent(type, dispatchingDevice)
, m_value(value)
{
    assert(type == Type::SpatialAxis);
}

const glm::mat3 & AxisEvent::value() const
{
    return m_value;
}

std::string AxisEvent::asString() const
{
    return std::to_string(static_cast<int>(m_type));
}


} // namespace gloperate
