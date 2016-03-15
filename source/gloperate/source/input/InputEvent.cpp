
#include <gloperate/input/InputEvent.h>

#include <cassert>

#include <glm/gtx/string_cast.hpp>


namespace gloperate
{


std::string InputEvent::typeToString(InputEvent::Type type)
{
    switch(type)
    {
    case Type::ButtonPress:
        return "ButtonPress";
    case Type::ButtonRelease:
        return "ButtonRelease";
    case Type::MouseButtonPress:
        return "MouseButtonPress";
    case Type::MouseButtonRelease:
        return "MouseButtonPress";
    case Type::MouseWheelScroll:
        return "MouseWheelScroll";
    case Type::SpatialAxis:
        return "SpatialAxis";
    default:
        return "UnknownType";
    }
    /*enum class Type
    {
        ButtonPress
      , ButtonRelease
      , MouseMove
      , MouseButtonPress
      , MouseButtonRelease
      , MouseWheelScroll
      , SpatialAxis
    };*/

}

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


} // namespace gloperate
