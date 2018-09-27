
#include <gloperate/input/InputEvent.h>

#include <cassert>

#include <glm/gtx/string_cast.hpp>


namespace gloperate
{


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

AbstractDevice * InputEvent::device() const
{
    return m_dispatchingDevice;
}

std::string InputEvent::asString() const
{
    return std::to_string(static_cast<int>(m_type));
}


} // namespace gloperate
