
#include <gloperate-glfw/WindowEventDispatcher.h>

#include <cassert>
#include <cmath>

#include <glbinding/gl/enum.h>

#include <GLFW/glfw3.h>

#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEvent.h>


namespace gloperate_glfw
{


void WindowEventDispatcher::registerWindow(Window * window)
{
    assert(window != nullptr);

    // Get GLFW window
    GLFWwindow * glfwWindow = window->internalWindow();
    if (!glfwWindow)
    {
        return;
    }

    // Connect GLFW window to window object
    glfwSetWindowUserPointer(glfwWindow, window);

    // Set callback functions
    glfwSetWindowRefreshCallback(glfwWindow, handleRefresh);
    glfwSetKeyCallback(glfwWindow, handleKey);
    glfwSetCharCallback(glfwWindow, handleChar);
    glfwSetMouseButtonCallback(glfwWindow, handleMouse);
    glfwSetCursorPosCallback(glfwWindow, handleCursorPos);
    glfwSetCursorEnterCallback(glfwWindow, handleCursorEnter);
    glfwSetScrollCallback(glfwWindow, handleScroll);
    glfwSetWindowSizeCallback(glfwWindow, handleResize);
    glfwSetFramebufferSizeCallback(glfwWindow, handleFramebufferResize);
    glfwSetWindowFocusCallback(glfwWindow, handleFocus);
    glfwSetWindowPosCallback(glfwWindow, handleMove);
    glfwSetWindowIconifyCallback(glfwWindow, handleIconify);
    glfwSetWindowCloseCallback(glfwWindow, handleClose);
}

void WindowEventDispatcher::deregisterWindow(Window * window)
{
    // Window pointer must be valid
    assert(window != nullptr);

    // Get GLFW window
    GLFWwindow * glfwWindow = window->internalWindow();
    if (!glfwWindow)
    {
        return;
    }

    // Deregister callbacks
    glfwSetWindowRefreshCallback(glfwWindow, nullptr);
    glfwSetKeyCallback(glfwWindow, nullptr);
    glfwSetCharCallback(glfwWindow, nullptr);
    glfwSetMouseButtonCallback(glfwWindow, nullptr);
    glfwSetCursorPosCallback(glfwWindow, nullptr);
    glfwSetCursorEnterCallback(glfwWindow, nullptr);
    glfwSetScrollCallback(glfwWindow, nullptr);
    glfwSetWindowSizeCallback(glfwWindow, nullptr);
    glfwSetFramebufferSizeCallback(glfwWindow, nullptr);
    glfwSetWindowFocusCallback(glfwWindow, nullptr);
    glfwSetWindowPosCallback(glfwWindow, nullptr);
    glfwSetWindowIconifyCallback(glfwWindow, nullptr);
    glfwSetWindowCloseCallback(glfwWindow, nullptr);
}

void WindowEventDispatcher::dispatchEvent(GLFWwindow * glfwWindow, WindowEvent * event)
{
    // Find window object from GLFW window, dispatch event
    dispatchEvent(fromGLFW(glfwWindow), event);
}

void WindowEventDispatcher::dispatchEvent(Window * window, WindowEvent * event)
{
    // Check if event is valid
    if (!event)
    {
        return;
    }

    // Check if window is valid, ignore event otherwise
    if (!window)
    {
        delete event;
        return;
    }

    // Dispatch event to window
    window->queueEvent(event);
}

Window * WindowEventDispatcher::fromGLFW(GLFWwindow * glfwWindow)
{
    if (glfwWindow) {
        return static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    } else {
        return nullptr;
    }
}

glm::ivec2 WindowEventDispatcher::mousePosition(GLFWwindow * glfwWindow)
{
    if (glfwWindow)
    {
        double xd, yd;
        glfwGetCursorPos(glfwWindow, &xd, &yd);

        return glm::ivec2(std::floor(xd), std::floor(yd));
    }
    else
    {
        return glm::ivec2();
    }
}

void WindowEventDispatcher::handleRefresh(GLFWwindow * glfwWindow)
{
    dispatchEvent(glfwWindow, new PaintEvent);
}

void WindowEventDispatcher::handleKey(GLFWwindow * glfwWindow, int key, int scanCode, int action, int modifiers)
{
    dispatchEvent(glfwWindow, new KeyEvent(key, scanCode, action, modifiers));
}

void WindowEventDispatcher::handleChar(GLFWwindow * glfwWindow, unsigned int character)
{
    dispatchEvent(glfwWindow, new KeyEvent(character));
}

void WindowEventDispatcher::handleMouse(GLFWwindow * glfwWindow, int button, int action, int modifiers)
{
    dispatchEvent(glfwWindow, new MouseEvent(mousePosition(glfwWindow), button, action, modifiers));
}

void WindowEventDispatcher::handleCursorPos(GLFWwindow * glfwWindow, double xPos, double yPos)
{
    dispatchEvent(glfwWindow, new MouseEvent(glm::ivec2(std::floor(xPos), std::floor(yPos))));
}

void WindowEventDispatcher::handleCursorEnter(GLFWwindow * glfwWindow, int entered)
{
    if (entered == GL_TRUE)
    {
        dispatchEvent(glfwWindow, new MouseEnterEvent);
    }
    else
    {
        dispatchEvent(glfwWindow, new MouseLeaveEvent);
    }
}

void WindowEventDispatcher::handleScroll(GLFWwindow * glfwWindow, double xOffset, double yOffset)
{
    dispatchEvent(glfwWindow, new ScrollEvent(glm::vec2(xOffset, yOffset), mousePosition(glfwWindow)));
}

void WindowEventDispatcher::handleResize(GLFWwindow * glfwWindow, int width, int height)
{
    dispatchEvent(glfwWindow, new ResizeEvent(glm::ivec2(width, height)));
}

void WindowEventDispatcher::handleFramebufferResize(GLFWwindow * glfwWindow, int width, int height)
{
    dispatchEvent(glfwWindow, new ResizeEvent(glm::ivec2(width, height), true));
}

void WindowEventDispatcher::handleMove(GLFWwindow * glfwWindow, int x, int y)
{
    dispatchEvent(glfwWindow, new MoveEvent(glm::ivec2(x, y)));
}

void WindowEventDispatcher::handleFocus(GLFWwindow * glfwWindow, int focused)
{
    dispatchEvent(glfwWindow, new FocusEvent(focused == GL_TRUE));
}

void WindowEventDispatcher::handleIconify(GLFWwindow * glfwWindow, int iconified)
{
    dispatchEvent(glfwWindow, new IconifyEvent(iconified == GL_TRUE));
}

void WindowEventDispatcher::handleClose(GLFWwindow * glfwWindow)
{
    dispatchEvent(glfwWindow, new CloseEvent);
}


} // namespace gloperate_glfw
