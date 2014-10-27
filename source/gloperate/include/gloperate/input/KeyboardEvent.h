#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractEvent.h>

#include <gloperate/input/input.h>

namespace gloperate
{

class GLOPERATE_API KeyboardEvent : public AbstractEvent
{
public:
    KeyboardEvent(Key key, int scanCode, int action, int modifiers);
    explicit KeyboardEvent(unsigned int character);

    int key() const;
    int scanCode() const;
    int action() const;
    int modifiers() const;
    unsigned int character() const;

protected:
    Key m_key;
    int m_scanCode;
    int m_action;
    int m_modifiers;
    unsigned int m_character;
};


} // namespace gloperate
