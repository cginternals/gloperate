
#pragma once


#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/input.h>


namespace gloperate
{


/**
*  @brief
*    Keyboard event
*
*    Informs about key press and release events
*/
class GLOPERATE_API KeyboardEvent : public AbstractEvent
{
public:
    /**
    *  @brief
    *    Keyboard event type (press or release)
    */
    enum class Type : char
    {
        Press
    ,   Release
    };


public:
    KeyboardEvent(Type type, Key key, int scanCode, int modifiers = 0);
    KeyboardEvent(Type type, unsigned int character);

    Type type() const;
    Key key() const;
    int scanCode() const;
    int modifiers() const;
    unsigned int character() const;


protected:
    Type m_type;
    Key m_key;
    int m_scanCode;
    int m_modifiers;
    unsigned int m_character;
};


} // namespace gloperate
