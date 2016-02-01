
#include <gloperate-glfw/WindowEventDispatcher2.h>

#include <cassert>
#include <cmath>

#include <glbinding/gl/enum.h>

#include <GLFW/glfw3.h>

#include <gloperate-glfw/WindowBase.h>
#include <gloperate-glfw/WindowEvent.h>


namespace gloperate_glfw
{


WindowEventDispatcher2::WindowTimerMap         WindowEventDispatcher2::s_timers;
std::chrono::high_resolution_clock::time_point WindowEventDispatcher2::s_time;
std::chrono::high_resolution_clock             WindowEventDispatcher2::s_clock;


WindowEventDispatcher2::Timer::Timer()
: interval(0)
, singleShot(false)
{
    reset();
}

WindowEventDispatcher2::Timer::Timer(int interval, bool singleShot)
: interval(interval)
, singleShot(singleShot)
{
    reset();
}

bool WindowEventDispatcher2::Timer::ready() const
{
    return elapsed >= interval;
}

void WindowEventDispatcher2::Timer::reset()
{
    elapsed = Duration(0);
}

void WindowEventDispatcher2::registerWindow(WindowBase * window)
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

void WindowEventDispatcher2::deregisterWindow(WindowBase * window)
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

    // Destroy open timers
    removeAllTimers(window);
}

void WindowEventDispatcher2::addTimer(WindowBase * window, int id, int interval, bool singleShot)
{
    // Create timer
    s_timers[window][id] = Timer(interval, singleShot);
}

void WindowEventDispatcher2::removeTimer(WindowBase * window, int id)
{
    // Remove timer
    s_timers[window].erase(id);
}

void WindowEventDispatcher2::removeAllTimers(WindowBase * window)
{
    // Remove all timers belonging to that window
    s_timers.erase(window);
}

void WindowEventDispatcher2::initializeTime()
{
    s_time = s_clock.now();
}

void WindowEventDispatcher2::checkForTimerEvents()
{
    // Get current time and calculate time difference since last call
    auto now = s_clock.now();
    Timer::Duration delta = std::chrono::duration_cast<Timer::Duration>(now - s_time);
    s_time = now;

    // Update all timers of all windows
    std::vector<std::pair<WindowBase *, int>> discarded;
    for (auto & timerMapPair : s_timers)
    {
        // Get window
        WindowBase * window = timerMapPair.first;
        for (auto & timerPair : timerMapPair.second)
        {
            // Get timer
            int id = timerPair.first;
            Timer & timer = timerPair.second;

            // Update timer
            timer.elapsed += delta;

            // Check if timer is active
            if (timer.ready())
            {
                // Send timer event to window
                dispatchEvent(window, new TimerEvent(id, timer.elapsed));

                // Reset timer (if continuous) or delete timer (if single-shot)
                if (timer.singleShot)
                {
                    discarded.emplace_back(window, id);
                }
                else
                {
                    timer.reset();
                }
            }
        }
    }

    // Remove all discarded timers
    for (const auto & pair : discarded)
    {
        removeTimer(pair.first, pair.second);
    }
}

void WindowEventDispatcher2::dispatchEvent(GLFWwindow * glfwWindow, WindowEvent * event)
{
    // Find window object from GLFW window, dispatch event
    dispatchEvent(fromGLFW(glfwWindow), event);
}

void WindowEventDispatcher2::dispatchEvent(WindowBase * window, WindowEvent * event)
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

WindowBase * WindowEventDispatcher2::fromGLFW(GLFWwindow * glfwWindow)
{
    if (glfwWindow) {
        return static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));
    } else {
        return nullptr;
    }
}

glm::ivec2 WindowEventDispatcher2::mousePosition(GLFWwindow * glfwWindow)
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

void WindowEventDispatcher2::handleRefresh(GLFWwindow * glfwWindow)
{
    dispatchEvent(glfwWindow, new PaintEvent);
}

void WindowEventDispatcher2::handleKey(GLFWwindow * glfwWindow, int key, int scanCode, int action, int modifiers)
{
    dispatchEvent(glfwWindow, new KeyEvent(key, scanCode, action, modifiers));
}

void WindowEventDispatcher2::handleChar(GLFWwindow * glfwWindow, unsigned int character)
{
    dispatchEvent(glfwWindow, new KeyEvent(character));
}

void WindowEventDispatcher2::handleMouse(GLFWwindow * glfwWindow, int button, int action, int modifiers)
{
    dispatchEvent(glfwWindow, new MouseEvent(mousePosition(glfwWindow), button, action, modifiers));
}

void WindowEventDispatcher2::handleCursorPos(GLFWwindow * glfwWindow, double xPos, double yPos)
{
    dispatchEvent(glfwWindow, new MouseEvent(glm::ivec2(std::floor(xPos), std::floor(yPos))));
}

void WindowEventDispatcher2::handleCursorEnter(GLFWwindow * glfwWindow, int entered)
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

void WindowEventDispatcher2::handleScroll(GLFWwindow * glfwWindow, double xOffset, double yOffset)
{
    dispatchEvent(glfwWindow, new ScrollEvent(glm::vec2(xOffset, yOffset), mousePosition(glfwWindow)));
}

void WindowEventDispatcher2::handleResize(GLFWwindow * glfwWindow, int width, int height)
{
    dispatchEvent(glfwWindow, new ResizeEvent(glm::ivec2(width, height)));
}

void WindowEventDispatcher2::handleFramebufferResize(GLFWwindow * glfwWindow, int width, int height)
{
    dispatchEvent(glfwWindow, new ResizeEvent(glm::ivec2(width, height), true));
}

void WindowEventDispatcher2::handleMove(GLFWwindow * glfwWindow, int x, int y)
{
    dispatchEvent(glfwWindow, new MoveEvent(glm::ivec2(x, y)));
}

void WindowEventDispatcher2::handleFocus(GLFWwindow * glfwWindow, int focused)
{
    dispatchEvent(glfwWindow, new FocusEvent(focused == GL_TRUE));
}

void WindowEventDispatcher2::handleIconify(GLFWwindow * glfwWindow, int iconified)
{
    dispatchEvent(glfwWindow, new IconifyEvent(iconified == GL_TRUE));
}

void WindowEventDispatcher2::handleClose(GLFWwindow * glfwWindow)
{
    dispatchEvent(glfwWindow, new CloseEvent);
}


} // namespace gloperate_glfw
