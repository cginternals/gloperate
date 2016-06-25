
#pragma once

#include <set>
#include <string>
#include <queue>

#include <glm/glm.hpp>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;
struct GLFWmonitor;


namespace gloperate
{
    class GLContextFormat;
}


namespace gloperate_glfw
{


class GLContext;
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


/**
*  @brief
*    Window with OpenGL context
*/
class GLOPERATE_GLFW_API Window
{
    friend class Application;
    friend class WindowEventDispatcher;


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
    *    Set OpenGL context format
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *
    *  @return
    *    'true' if the format could be set, else 'false'
    *
    *  @remarks
    *    The context format can only be set before the window
    *    has been created. Afterwards, the function will fail
    *    and the context format will not be changed.
    */
    bool setContextFormat(const gloperate::GLContextFormat & format);

    /**
    *  @brief
    *    Create window
    *
    *  @return
    *    'true' if window could be created, else 'false'
    *
    *  @remarks
    *    If the window has already been created, this function will fail.
    */
    bool create();

    /**
    *  @brief
    *    Destroy window
    */
    void destroy();

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be nullptr)
    */
    GLContext * context() const;

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
    void setSize(int width, int height);

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


protected:
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
    *    Check for repaint event
    *
    *  @remarks
    *    If repaint() has been called on the window, a repaint event
    *    will be added to the window's event queue. This needs to be
    *    done as a separate call to avoid event processing to get stuck
    *    in an endless loop. Make sure to call Application::pollEvents()
    *    and updateRepaintEvent() in turn.
    */
    void updateRepaintEvent();

    /**
    *  @brief
    *    Handle window event
    *
    *  @param[in] event
    *    Window event
    */
    void handleEvent(WindowEvent & event);

    /**
    *  @brief
    *    Remove all waiting events from the queue
    */
    void clearEventQueue();

    /**
    *  @brief
    *    Create internal window and OpenGL context
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
    bool createInternalWindow(const gloperate::GLContextFormat & format, int width, int height, GLFWmonitor * monitor = nullptr);

    /**
    *  @brief
    *    Destroy internal window and OpenGL context
    *
    *  @remarks
    *    This function will actually destroy the current window.
    */
    void destroyInternalWindow();

    // Event handlers, to be overwritten in derived classes
    virtual void onContextInit();
    virtual void onContextDeinit();
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
    GLFWwindow * m_window;                  ///< GLFW window (can be nullptr)
    GLContext  * m_context;                 ///< OpenGL context (can be nullptr)
    WindowMode   m_windowMode;              ///< Window mode (windowed or fullscreen)
    glm::ivec2   m_windowedModeSize;        ///< Size of window when returned from fullscreen mode
    bool         m_quitOnDestroy;           ///< Quit application when window is closed?
    std::string  m_title;                   ///< Window title

    gloperate::GLContextFormat m_format;    ///< The desired OpenGL context format

    std::queue<WindowEvent*> m_eventQueue;  ///< List of events to be processed by the window
    bool m_needsRepaint;                    ///< Has a repaint be scheduled?


private:
    static std::set<Window*> s_instances;   ///< List of window instances
};


} // namespace gloperate_glfw
