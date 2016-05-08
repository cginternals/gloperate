
#pragma once


#include <gloperate/gloperate_api.h>
#include <glm/detail/type_vec2.hpp>
#include <gloperate/input/input.h>


namespace gloperate
{
class AbstractInteraction;

struct GLOPERATE_API KeyboardInteractionArgs {
    Key key;
    int scanCode;
    int modifiers;
    KeyboardInteractionArgs(Key key, int scanCode, int modifiers) : key(key), scanCode(scanCode), modifiers(modifiers) {};
};

struct GLOPERATE_API InteractionArgs {
    glm::ivec2 position;
    InteractionArgs(glm::ivec2 position) : position(position){};
};

struct GLOPERATE_API MouseInteractionArgs : public InteractionArgs {
    MouseButton button;
    MouseInteractionArgs(glm::ivec2 position, MouseButton button) : InteractionArgs(position), button{button} {};
};

struct GLOPERATE_API MouseInteractionDragArgs : public MouseInteractionArgs
{
    AbstractInteraction *hoverElement;
    MouseInteractionDragArgs(glm::ivec2 position, MouseButton button, AbstractInteraction *hoverElement)
        : MouseInteractionArgs(position, button),
        hoverElement(hoverElement)
    {};
};

struct GLOPERATE_API MouseInteractionDragDeltaArgs: public MouseInteractionDragArgs {
    glm::ivec2 cummulativeDelta;
    glm::ivec2 startPosition;
    MouseInteractionDragDeltaArgs(glm::ivec2 position, MouseButton button, AbstractInteraction *hoverElement, glm::ivec2 cummulativeDelta, glm::ivec2 startPosition) 
        : MouseInteractionDragArgs(position, button, hoverElement),
        cummulativeDelta(cummulativeDelta),
        startPosition(startPosition)
    {};
};

struct GLOPERATE_API MouseInteractionWheelArgs : public InteractionArgs {
    glm::vec2 angleDelta;
    MouseInteractionWheelArgs(glm::ivec2 position, glm::vec2 angleDelta) : InteractionArgs(position), angleDelta(angleDelta) {};
};

/**
*  @brief
*    Base class for interaction techniques
*/
class GLOPERATE_API AbstractInteraction
{
public:
    AbstractInteraction();
    virtual ~AbstractInteraction();
    virtual void dragStart(MouseInteractionArgs e) {};
    virtual void dragDelta(MouseInteractionDragDeltaArgs e) {};
    virtual void dragEnd(MouseInteractionArgs e) {};
    virtual void mouseWheel(MouseInteractionWheelArgs e) {};
    virtual void click(MouseInteractionArgs e) {};
    virtual void keyPress(KeyboardInteractionArgs e) {};
};


} // namespace gloperate
