
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
    MouseEvent(Type type, const glm::ivec2 & pos, MouseButton button = NoMouseButton, int modifiers = 0);

    Type type() const;
    MouseButton button() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;


protected:
    Type m_type;
    MouseButton m_button;
    int m_modifiers;
    glm::ivec2 m_pos;
};


} // namespace gloperate
