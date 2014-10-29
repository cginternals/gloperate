#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractEvent.h>

#include <gloperate/input/input.h>

namespace gloperate
{

class GLOPERATE_API KeyboardEvent : public AbstractEvent
{
public:
    KeyboardEvent(EventType eventType, Key key, int scanCode, int modifiers = 0);
    explicit KeyboardEvent(EventType eventType, unsigned int character);

    Key key() const;
    int scanCode() const;
    int modifiers() const;
    unsigned int character() const;

protected:
    Key m_key;
    int m_scanCode;
    int m_modifiers;
    unsigned int m_character;
};


} // namespace gloperate
