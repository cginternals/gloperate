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
    MouseEvent(EventType eventType, const glm::ivec2 & pos, MouseButton button = NoMouseButton, int modifiers = 0);

    MouseButton button() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;

protected:
    MouseButton m_button;
    int m_modifiers;

    glm::ivec2 m_pos;
};

} // namespace gloperate
