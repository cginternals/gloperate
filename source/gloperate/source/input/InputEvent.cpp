
#include <cassert>

#include <gloperate/input/InputEvent.h>

namespace gloperate{

InputEvent::InputEvent(MouseButton _mouseButton, int _xPos, int _yPos, EventType _action) :
  m_type(InputEventType::MouseEvent)
, mouseContent{_mouseButton, _xPos, _yPos}
, m_action(_action)
{

}


InputEvent::InputEvent(KeyModifier _keyModifier, Key _key, EventType _action) :
  m_type(InputEventType::KeyboardEvent)
, keyboardContent{_keyModifier, _key}
, m_action(_action)
{

}

InputEventType InputEvent::type() const
{
    return m_type;
}

EventType InputEvent::action() const
{
    return m_action;
}

MouseButton InputEvent::mouseButton() const
{
    assert(m_type == InputEventType::MouseEvent);
    return mouseContent.mouseButton;
}

int InputEvent::xPos() const
{
    assert(m_type == InputEventType::MouseEvent);
    return mouseContent.xPos;
}

int InputEvent::yPos() const
{
    assert(m_type == InputEventType::MouseEvent);
    return mouseContent.yPos;
}

KeyModifier InputEvent::keyModifier() const
{
    assert(m_type == InputEventType::KeyboardEvent);
    return keyboardContent.keyModifier;
}

Key InputEvent::key() const
{
    return keyboardContent.key;
}

} //namespace gloperate
