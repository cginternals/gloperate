#include <gloperate/input/KeyboardEvent.h>


namespace gloperate
{

KeyboardEvent::KeyboardEvent(EventType eventType, unsigned int character)
: AbstractEvent(eventType)
, m_key(KeyUnknown)
, m_scanCode(0)
, m_modifiers(0)
, m_character(character)
{
    m_sourceType = SourceType::Keyboard;
}

KeyboardEvent::KeyboardEvent(EventType eventType, Key key, int scanCode, int modifiers)
: AbstractEvent(eventType)
, m_key(key)
, m_scanCode(scanCode)
, m_modifiers(modifiers)
, m_character(0)
{
    m_sourceType = SourceType::Keyboard;
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
