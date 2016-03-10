
#include <gloperate/input/ButtonEvent.h>

namespace gloperate{

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
