
#include <gloperate/input/ButtonEvent.h>


namespace gloperate
{


ButtonEvent::ButtonEvent(Type type, AbstractDevice * dispatchingDevice, int key, int modifier)
: InputEvent(type, dispatchingDevice)
, m_key(key)
, m_modifier(modifier)
{
    assert(type == Type::ButtonPress ||
           type == Type::ButtonRelease);
}


int ButtonEvent::key() const
{
    return m_key;
}


int ButtonEvent::modifier() const
{
    return m_modifier;
}


std::string ButtonEvent::asString() const
{
    return (type() == Type::ButtonPress ? "Press " : "Release ") + std::to_string(m_key) + ":" + std::to_string(m_modifier);
}


} // namespace gloperate
