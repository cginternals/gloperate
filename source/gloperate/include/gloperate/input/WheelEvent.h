#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractEvent.h>

#include <glm/glm.hpp>

#include <gloperate/input/input.h>

namespace gloperate
{

class GLOPERATE_API WheelEvent : public AbstractEvent
{
public:
    WheelEvent(
        const glm::ivec2 & pos, 
        const glm::vec2 & angleDelta, 
        const glm::ivec2 & pixelDelta, 
        MouseButton button = NoMouseButton);

    MouseButton button() const;
    const glm::vec2 & angleDelta() const;
    const glm::ivec2 & pixelDelta() const;
    const glm::ivec2 & pos() const;

    int x() const;
    int y() const;
    
    /**
    *  @brief
    *    A helpful value when using mouse wheel for scaling.
    */
    static float defaultMouseAngleDelta();

protected:
    MouseButton m_button;
    glm::vec2 m_angleDelta;
    glm::ivec2 m_pixelDelta;
    glm::ivec2 m_pos;

};

} // namespace gloperate
