
#include <gloperate/input/KeyboardEvent.h>


namespace gloperate
{


KeyboardEvent::KeyboardEvent(Type type, unsigned int character)
: AbstractEvent()
, m_type(type)
, m_key(Key::KeyUnknown)
, m_scanCode(0)
, m_modifiers(0)
, m_character(character)
{
    m_sourceType = EventType::Keyboard;
}

KeyboardEvent::KeyboardEvent(KeyboardEvent::Type type, Key key, int scanCode, int modifiers)
: AbstractEvent()
, m_type(type)
, m_key(key)
, m_scanCode(scanCode)
, m_modifiers(modifiers)
, m_character(0)
{
    m_sourceType = EventType::Keyboard;
}

KeyboardEvent::Type KeyboardEvent::type() const
{
    return m_type;
}

Key KeyboardEvent::key() const
{
    return m_key;
}

int KeyboardEvent::scanCode() const
{
    return m_scanCode;
}

int KeyboardEvent::modifiers() const
{
    return m_modifiers;
}

unsigned int KeyboardEvent::character() const
{
    return m_character;
}


} // namespace gloperate
