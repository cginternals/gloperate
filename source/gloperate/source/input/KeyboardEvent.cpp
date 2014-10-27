#include <gloperate/input/KeyboardEvent.h>


namespace gloperate
{

KeyboardEvent::KeyboardEvent(unsigned int character)
: AbstractEvent()
, m_key(0)
, m_scanCode(0)
, m_action(0)
, m_modifiers(0)
, m_character(character)
{
}

KeyboardEvent::KeyboardEvent(int key, int scanCode, int action, int modifiers)
: AbstractEvent()
, m_key(key)
, m_scanCode(scanCode)
, m_action(action)
, m_modifiers(modifiers)
, m_character(0)
{
}

int KeyboardEvent::key() const
{
    return m_key;
}

int KeyboardEvent::scanCode() const
{
    return m_scanCode;
}

int KeyboardEvent::action() const
{
    return m_action;
}

int KeyboardEvent::modifiers() const
{
    return m_modifiers;
}

unsigned int KeyboardEvent::character() const
{
    return m_character;
}

} // namespace gloperate_glfw
