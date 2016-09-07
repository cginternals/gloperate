
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/input.h>


namespace gloperate
{


/**
*  @brief
*    Mouse event
*
*    Informs about mouse events such as mouse movement and mouse buttons
*/
class GLOPERATE_API MouseEvent : public AbstractEvent
{
public:
    /**
    *  @brief
    *    Mouse event type
    */
    enum class Type : char
    {
        Press
    ,   Release
    ,   Move
    ,   Enter
    ,   Leave
    ,   DoubleClick
    };

public:    
    MouseEvent(Type type, const glm::ivec2 & pos, const glm::ivec2 & lastPos, const glm::ivec2 & screenSize, MouseButton button = MouseButton::NoMouseButton, MouseButton buttonMask = MouseButton::NoMouseButton, KeyModifier modifiers = KeyModifier::ModNone);

    Type type() const;
    MouseButton button() const;
    MouseButton buttonMask() const;
    KeyModifier modifiers() const;

    glm::ivec2 position() const;
    glm::ivec2 lastPosition() const;
    glm::ivec2 positionDelta() const;

    glm::vec2 normalizedPosition() const;
    glm::vec2 normalizedLastPosition() const;
    glm::vec2 normalizedPositionDelta() const;

protected:
    glm::vec2 normalize(const glm::ivec2 & pos) const;


protected:
    Type m_type;
    MouseButton m_button;
    MouseButton m_buttonMask;
    KeyModifier m_modifiers;

    glm::ivec2 m_pos;
    glm::ivec2 m_lastPos;
    glm::vec2 m_screenSize;
};


} // namespace gloperate
