
#include <gloperate-glfw/input.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


using namespace gloperate;


namespace gloperate_glfw
{


gloperate::MouseButton fromGLFWMouseButton(int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_1: return MouseButton1;
        case GLFW_MOUSE_BUTTON_2: return MouseButton2;
        case GLFW_MOUSE_BUTTON_3: return MouseButton3;
        case GLFW_MOUSE_BUTTON_4: return MouseButton4;
        case GLFW_MOUSE_BUTTON_5: return MouseButton5;
        case GLFW_MOUSE_BUTTON_6: return MouseButton6;
        case GLFW_MOUSE_BUTTON_7: return MouseButton7;
        case GLFW_MOUSE_BUTTON_8: return MouseButton8;
        default:                  return NoMouseButton;
    }
}

gloperate::Key fromGLFWKeyCode(int key)
{
    // We are using the same key code table as GLFW
    return static_cast<gloperate::Key>(key);
}

gloperate::KeyModifier fromGLFWModifier(int modifier)
{
    //We are using the same modifier code table as GLFW
    return static_cast<gloperate::KeyModifier>(modifier);
}

} // namespace gloperate_glfw
