#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractEvent.h>

#include <glm/glm.hpp>

#include <gloperate/input/input.h>

namespace gloperate
{

class GLOPERATE_API MouseEvent : public AbstractEvent
{

public:
    enum class Type : char
    {
        Press
    ,   Release
    ,   Move
    ,   Enter
    ,   Leave
    ,   DoubleClick
    };

    MouseEvent(Type type, const glm::ivec2 & pos, MouseButton button = NoMouseButton, MouseButton buttonMask = NoMouseButton, int modifiers = 0);

    Type type() const;
    MouseButton button() const;
    MouseButton buttonMask() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;

protected:
    Type m_type;
    MouseButton m_button;
    MouseButton m_buttonMask;
    int m_modifiers;

    glm::ivec2 m_pos;
};

} // namespace gloperate
