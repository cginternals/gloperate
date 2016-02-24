
#pragma once

#include <string>

#include <gloperate/gloperate_api.h>
#include <gloperate/viewer/input.h>

namespace gloperate{

enum class InputEventType {
    MouseEvent,
    KeyboardEvent
};

enum class EventType {
    Press,
    Release,

};


class GLOPERATE_API InputEvent
{
public:
    InputEvent(MouseButton _mouseButton, int _xPos, int _yPos, EventType _action);
    InputEvent(KeyModifier _keyModifier, Key _key, EventType _action);

    InputEventType type() const;
    EventType action() const;

    MouseButton mouseButton() const;
    int xPos() const;
    int yPos() const;

    KeyModifier keyModifier() const;
    Key key() const;

private:

    InputEventType m_type;
    EventType m_action;
    union
    {
        struct{ MouseButton mouseButton; int xPos; int yPos; } mouseContent;
        struct{ KeyModifier keyModifier; Key key; } keyboardContent;
    };
};

} //namespace gloperate
