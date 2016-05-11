
#pragma once


#include <gloperate/gloperate_api.h>
#include <glm/detail/type_vec2.hpp>
#include <gloperate/input/input.h>


namespace gloperate
{
class AbstractInteraction;

struct GLOPERATE_API KeyboardInteractionArgs {
    int id;
    Key key;
    int scanCode;
    int modifiers;
};

struct GLOPERATE_API MouseInteractionArgs {
    int id;
    glm::ivec2 position;
    MouseButton button;
};

struct GLOPERATE_API MouseInteractionDragDeltaArgs {
    int id;
    glm::ivec2 position;
    MouseButton button;
    AbstractInteraction *hoverElement;
    glm::ivec2 cummulativeDelta;
    glm::ivec2 startPosition;
};

struct GLOPERATE_API MouseInteractionWheelArgs {
    int id;
    glm::ivec2 position;
    glm::vec2 angleDelta;
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
