#pragma once

#include <gloperate/gloperate_api.h>


namespace gloperate
{

class GLOPERATE_API MouseEvent : public AbstractEvent
{
public:
    MouseEvent(const glm::ivec2 & pos);
    MouseEvent(const glm::ivec2 & pos, int button, int action, int modifiers);

    int button() const;
    int action() const;
    int modifiers() const;

    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;

protected:
    int m_button;
    int m_action;
    int m_modifiers;

    glm::ivec2 m_pos;
};

} // namespace gloperate_glfw
