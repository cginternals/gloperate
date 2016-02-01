
#pragma once

#include <unordered_map>
#include <chrono>

#include <glm/glm.hpp>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;


namespace gloperate_glfw
{


class WindowBase;
class WindowEvent;


/**
*  @brief
*    Event dispatcher
*
*    Static class that handles the dispatching of
*    GLWF events to windows.
*/
class GLOPERATE_GLFW_API WindowEventDispatcher2
{
public:
    /**
    *  @brief
    *    Register window for event dispatching
    *
    *  @param[in] window
    *    Window (must be valid!)
    */
    static void registerWindow(WindowBase * window);

    /**
    *  @brief
    *    Unregister window from event dispatching
    *
    *  @param[in] window
    *    Window (must be valid!)
    */
    static void deregisterWindow(WindowBase * window);

    /**
    *  @brief
    *    Create timer on window
    *
    *  @param[in] window
    *    Window (must be valid!)
    *  @param[in] id
    *    Timer ID
    *  @param[in] interval
    *    Timer interval (in milliseconds)
    *  @param[in] singleShot
    *    If 'true', the timer fires only once, otherwise periodically
    */
    static void addTimer(WindowBase * window, int id, int interval, bool singleShot);

    /**
    *  @brief
    *    Remove timer on window
    *
    *  @param[in] window
    *    Window (must be valid!)
    *  @param[in] id
    *    Timer ID
    */
    static void removeTimer(WindowBase * window, int id);

    /**
    *  @brief
    *    Remove all timers on window
    *
    *  @param[in] window
    *    Window (must be valid!)
    */
    static void removeAllTimers(WindowBase * window);

    /**
    *  @brief
    *    Initialize clock for timers
    */
    static void initializeTime();

    /**
    *  @brief
    *    Update timers and generate timer events
    */
    static void checkForTimerEvents();


protected:
    /**
    *  @brief
    *    Dispatch event to window
    *
    *  @param[in] window
    *    GLFW window (can be nullptr)
    *  @param[in] event
    *    Event (can be nullptr)
    *
    *  @remarks
    *    The event object will be destroyed after dispatching.
    */
    static void dispatchEvent(GLFWwindow * glfwWindow, WindowEvent * event);

    /**
    *  @brief
    *    Dispatch event to window
    *
    *  @param[in] window
    *    Window (can be nullptr)
    *  @param[in] event
    *    Event (can be nullptr)
    *
    *  @remarks
    *    The event object will be destroyed after dispatching.
    */
    static void dispatchEvent(WindowBase * window, WindowEvent * event);

    /**
    *  @brief
    *    Find window object from GLFW window
    *
    *  @param[in] window
    *    GLFW window (can be nullptr)
    *
    *  @return
    *    Window (can be nullptr)
    */
    static WindowBase * fromGLFW(GLFWwindow * glfwWindow);

    /**
    *  @brief
    *    Get current mouse position inside the given window
    *
    *  @param[in] window
    *    GLFW window (can be nullptr)
    *
    *  @return
    *    Position, (0, 0) if mouse is not inside the window or window is invalid
    */
    static glm::ivec2 mousePosition(GLFWwindow * glfwWindow);

    // Event callbacks
    static void handleRefresh(GLFWwindow * glfwWindow);
    static void handleKey(GLFWwindow * glfwWindow, int key, int scanCode, int action, int modifiers);
    static void handleChar(GLFWwindow * glfwWindow, unsigned int character);
    static void handleMouse(GLFWwindow * glfwWindow, int button, int action, int modifiers);
    static void handleCursorPos(GLFWwindow * glfwWindow, double xPos, double yPos);
    static void handleCursorEnter(GLFWwindow * glfwWindow, int entered);
    static void handleScroll(GLFWwindow * glfwWindow, double xOffset, double yOffset);
    static void handleResize(GLFWwindow * glfwWindow, int width, int height);
    static void handleFramebufferResize(GLFWwindow * glfwWindow, int width, int height);
    static void handleMove(GLFWwindow * glfwWindow, int x, int y);
    static void handleFocus(GLFWwindow * glfwWindow, int focused);
    static void handleIconify(GLFWwindow * glfwWindow, int iconified);
    static void handleClose(GLFWwindow * glfwWindow);


private:
    /**
    *  @brief
    *    Constructor
    */
    WindowEventDispatcher2();


private:
    /**
    *  @brief
    *    Timer to dispatch periodic events
    */
    struct Timer
    {
        using Duration = std::chrono::duration<double, std::milli>;

        Timer();
        Timer(int interval, bool singleShot);

        bool ready() const;
        void reset();

        std::chrono::milliseconds interval;
        Duration elapsed;
        bool singleShot;
    };

    // Convenience data types
    using TimerMap = std::unordered_map<int, Timer>;
    using WindowTimerMap = std::unordered_map<WindowBase*, TimerMap>;


private:
    static WindowTimerMap                                 s_timers; ///< Map window -> list of timers
    static std::chrono::high_resolution_clock::time_point s_time;   ///< Current time
    static std::chrono::high_resolution_clock             s_clock;  ///< Time measurement
};


} // namespace gloperate_glfw
