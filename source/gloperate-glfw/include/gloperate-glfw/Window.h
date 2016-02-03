
#pragma once

#include <set>
#include <string>
#include <queue>

#include <glm/glm.hpp>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;
struct GLFWmonitor;


namespace gloperate
{
    class ContextFormat;
}


namespace gloperate_glfw
{


class Context;
class WindowEvent;
class KeyEvent;
class MouseEvent;
class MouseEnterEvent;
class MouseLeaveEvent;
class ScrollEvent;
class ResizeEvent;
class PaintEvent;
class FocusEvent;
class IconifyEvent;
class MoveEvent;
class TimerEvent;


/**
*  @brief
*    Window with OpenGL context
*/
class GLOPERATE_GLFW_API Window
{
public:
    /**
    *  @brief
    *    Get list of windows
    *
    *  @return
    *    List of open windows
    */
    static const std::set<Window*> & instances();


public:
    /**
    *  @brief
    *    Constructor
    */
    Window();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Window();

    /**
    *  @brief
    *    Create window
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *  @param[in] width
    *    Window width (in pixels)
    *  @param[in] height
    *    Window height (in pixels)
    *
    *  @return
    *    'true' if window could be created, else 'false'
    *
    *  @remarks
    *    If the window has already been created, this function will fail
    */
    bool create(
        const gloperate::ContextFormat & format
      , int width  = 1280
      , int height = 720);

    /**
    *  @brief
    *    Create window
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *  @param[in] title
    *    Window title
    *  @param[in] width
    *    Window width (in pixels)
    *  @param[in] height
    *    Window height (in pixels)
    *
    *  @return
    *    'true' if window could be created, else 'false'
    *
    *  @remarks
    *    If the window has already been created, this function will fail
    */
    bool create(
        const gloperate::ContextFormat & format
      , const std::string & title = "gloperate"
      , int width  = 1280
      , int height =  720);

    /**
    *  @brief
    *    Destroy window
    */
    void destroy();

    /**
    *  @brief
    *    Get GLFW window handle
    *
    *  @return
    *    GLFW window, can be nullptr
    */
    GLFWwindow * internalWindow() const;

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context, can be nullptr
    */
    Context * context() const;

    /**
    *  @brief
    *    Show window (make it visible)
    */
    void show();

    /**
    *  @brief
    *    Hide window (make it invisible)
    */
    void hide();

    /**
    *  @brief
    *    Close window
    */
    void close();

    /**
    *  @brief
    *    Check if window is in fullscreen-mode
    *
    *  @return
    *    'true' if window is in fullscreen-mode, else 'false'
    */
    bool isFullscreen() const;

    /**
    *  @brief
    *    Set fullscreen-mode
    *
    *  @param[in] fullscreen
    *    'true' to switch to fullscreen-mode, 'false' to switch to windowed-mode
    */
    void setFullscreen(bool fullscreen);

    /**
    *  @brief
    *    Get window position
    *
    *  @return
    *    Window position (in pixels)
    */
    glm::ivec2 position() const;

    /**
    *  @brief
    *    Get window size
    *
    *  @return
    *    Window size (in pixels)
    */
    glm::ivec2 size() const;

    /**
    *  @brief
    *    Set window size
    *
    *  @param[in] width
    *    Window width (in pixels)
    *  @param[in] height
    *    Window height (in pixels)
    */
    void resize(int width, int height);

    /**
    *  @brief
    *    Get framebuffer size
    *
    *  @return
    *    Framebuffer size (in device pixels, may be larger than virtual window resolution)
    */
    glm::ivec2 framebufferSize() const;

    /**
    *  @brief
    *    Get window title
    *
    *  @return
    *    Window title
    */
    const std::string & title() const;

    /**
    *  @brief
    *    Set window title
    *
    *  @param[in] title
    *    Window title
    */
    void setTitle(const std::string & title);

    /**
    *  @brief
    *    Query input mode
    *
    *  @param[in] mode
    *    Input mode
    *
    *  @return
    *    Value of input mode
    *
    *  @notes
    *    - [TODO] What modes and values are supported here??
    */
    int inputMode(int mode) const;

    /**
    *  @brief
    *    Set input mode
    *
    *  @param[in] mode
    *    Input mode
    *  @param[in] value
    *    Input mode value
    *
    *  @notes
    *    - [TODO] What modes and values are supported here??
    */
    void setInputMode(int mode, int value);

    /**
    *  @brief
    *    Check if application quits when windows gets closed
    *
    *  @return
    *    'true' if window quits the application when closed, else 'false'
    */
    bool quitsOnDestroy() const;

    /**
    *  @brief
    *    Set if application quits when the window gets closed
    *
    *  @param[in] quitOnDestroy
    *    'true' if window quits the application when closed, else 'false'
    *
    *  @remarks
    *    If enabled, this causes an application wide quit message to be posted
    *    when the window gets destroyed. Hence, the MainLoop will be quit
    *    and all other remaining windows destroyed.
    */
    void setQuitOnDestroy(bool quitOnDestroy);

    /**
    *  @brief
    *    Schedule a repaint on the window
    *
    *  @remarks
    *    When calling this function, a redraw-event will be sent to the
    *    message queue, causing the window to be repainted within the
    *    next event processing.
    */
    void repaint();

    /**
    *  @brief
    *    Swap front and back buffer
    */
    void swap();

    /**
    *  @brief
    *    Create timer on window
    *
    *  @param[in] id
    *    Timer ID
    *  @param[in] interval
    *    Timer interval (in milliseconds)
    *  @param[in] singleShot
    *    If 'true', the timer fires only once, otherwise periodically
    */
    void addTimer(int id, int interval, bool singleShot = false);

    /**
    *  @brief
    *    Remove timer from window
    *
    *  @param[in] id
    *    Timer ID
    */
    void removeTimer(int id);

    /**
    *  @brief
    *    Add event to the window's event queue
    *
    *  @param[in] event
    *    Event (can be nullptr)
    */
    void queueEvent(WindowEvent * event);

    /**
    *  @brief
    *    Check if window has events waiting
    *
    *  @return
    *    'true' if events are waiting in the window's event queue, else 'false'
    */
    bool hasPendingEvents();

    /**
    *  @brief
    *    Process all waiting events and dispatch them to event handling
    */
    void processEvents();

    /**
    *  @brief
    *    Handle window event
    *
    *  @param[in] event
    *    Window event
    */
    void handleEvent(WindowEvent & event);

    void idle();


protected:
    // Event handlers, to be overwritten in derived classes
    virtual void onContextInit();
    virtual void onContextDeinit();
    virtual void onIdle();
    virtual void onResize(ResizeEvent & event);
    virtual void onFramebufferResize(ResizeEvent & event);
    virtual void onMove(MoveEvent & event);
    virtual void onPaint(PaintEvent & event);
    virtual void onKeyPress(KeyEvent & event);
    virtual void onKeyRelease(KeyEvent & event);
    virtual void onMousePress(MouseEvent & event);
    virtual void onMouseMove(MouseEvent & event);
    virtual void onMouseRelease(MouseEvent & event);
    virtual void onMouseEnter(MouseEnterEvent & event);
    virtual void onMouseLeave(MouseLeaveEvent & event);
    virtual void onScroll(ScrollEvent & event);
    virtual void onFocus(FocusEvent & event);
    virtual void onIconify(IconifyEvent & event);
    virtual void onTimer(TimerEvent & event);


protected:
    /**
    *  @brief
    *    Create OpenGL context (and window)
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *  @param[in] width
    *    Window width (in pixels)
    *  @param[in] height
    *    Window height (in pixels)
    *  @param[in] monitor
    *    GLWF monitor handle, can be nullptr
    *
    *  @return
    *    'true' if context could be created, else 'false'
    *
    *  @remarks
    *    This function will actually create a new window with the given context
    *    format, so any previously obtained window IDs will be rendered invalid.
    */
    bool createContext(const gloperate::ContextFormat & format, int width, int height, GLFWmonitor * monitor = nullptr);

    /**
    *  @brief
    *    Destroy OpenGL context (and window)
    *
    *  @remarks
    *    This function will actually destroy the current window.
    */
    void destroyContext();

    void initializeEventHandler();
    void finalizeEventHandler();

    void clearEventQueue();
    void processEvent(WindowEvent & event);
    void postprocessEvent(WindowEvent & event);


protected:
    /**
    *  @brief
    *    Window mode
    */
    enum WindowMode
    {
        WindowedMode    ///< Window is in windowed-mode
      , FullscreenMode  ///< Window is in fullscreen-mode
    };


protected:
    GLFWwindow * m_window;           ///< GLFW window (can be nullptr)
    Context    * m_context;          ///< OpenGL context (can be nullptr)
    WindowMode   m_windowMode;       ///< Window mode (windowed or fullscreen)
    glm::ivec2   m_windowedModeSize; ///< Size of window when returned from fullscreen mode
    bool         m_quitOnDestroy;    ///< Quit application when window is closed?
    std::string  m_title;            ///< Window title

    std::queue<WindowEvent*> m_eventQueue;  ///< List of events to be processed by the window


private:
    static std::set<Window*> s_instances;   ///< List of window instances
};


} // namespace gloperate_glfw
