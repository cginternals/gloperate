
#pragma once

#include <set>
#include <string>
#include <queue>

#include <glm/glm.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventHandlerBase.h>


struct GLFWwindow;
struct GLFWmonitor;


namespace gloperate
{
    class ContextFormat;
    class ResourceManager;
}


namespace gloperate_glfw
{


class WindowEvent;
class Context;


/**
*  @brief
*    Window for rendering
*
*  @remarks
*    Attach a WindowEventHandlerBase specialization for event handling.
*/
class GLOPERATE_GLFW_API Window
{
public:
    static int init();


public:
    Window(gloperate::ResourceManager & resourceManager);
    virtual ~Window();

    bool create(
        const gloperate::ContextFormat & format
    ,   int width  = 1280
    ,   int height =  720);

    bool create(
        const gloperate::ContextFormat & format
    ,   const std::string & title = "gloperate"
    ,   int width  = 1280
    ,   int height =  720);

    /**
    *  @brief
    *    Set event handler for this window
    *
    *  @remarks
    *    Takes ownership of the given eventhandler and deletes that either on
    *    quitting, just before the opengl context gets destroyed, or when
    *    reassigning a new, different handler.
    */
    void setEventHandler(WindowEventHandlerBase * eventHandler);

    WindowEventHandlerBase * eventHandler();
    const WindowEventHandlerBase * eventHandler() const;

    void close();

    void show();
    void hide();

    void setTitle(const std::string & title);
    const std::string & title() const;

    void resize(int width, int height);
    glm::ivec2 size() const;

    glm::ivec2 position() const;
    glm::ivec2 framebufferSize() const;

    int inputMode(int mode) const;
    void setInputMode(int mode, int value);

    /**
    *  @brief
    *    Set if application shall quit when the window has been destroyed
    *
    *  @remarks
    *    If enabled, this causes an application wide quit message to be posted
    *    when the window gets destroyed. Hence, the MainLoop will be quit
    *    and all other remaining windows destroyed.
    */
    void quitOnDestroy(bool enable);
    bool quitsOnDestroy() const;

    Context * context() const;

    void repaint();
    void idle();

    void fullScreen();
    bool isFullScreen() const;
    void windowed();
    bool isWindowed() const;
    void toggleMode();

    GLFWwindow * internalWindow() const;

    void queueEvent(WindowEvent * event);
    bool hasPendingEvents();
    void processEvents();

    static const std::set<Window*>& instances();

    void addTimer(int id, int interval, bool singleShot = false);
    void removeTimer(int id);

    void swap();
    void destroy();

    gloperate::Painter * painter() const;
    void setPainter(gloperate::Painter * painter);

    gloperate::ResourceManager & resourceManager();
    const gloperate::ResourceManager & resourceManager() const;


protected:
    bool createContext(const gloperate::ContextFormat & format, int width, int height, GLFWmonitor * monitor = nullptr);
    void destroyContext();

    void initializeEventHandler();
    void finalizeEventHandler();

    void clearEventQueue();
    void processEvent(WindowEvent & event);
    void postprocessEvent(WindowEvent & event);


protected:
    Context * m_context;
    GLFWwindow * m_window;

    globjects::ref_ptr<WindowEventHandlerBase> m_eventHandler;
    std::queue<WindowEvent*> m_eventQueue;
    glm::ivec2 m_windowedModeSize;

    bool m_quitOnDestroy;

    enum Mode
    {
        WindowMode
    ,   FullScreenMode
    };

    Mode m_mode;

    gloperate::Painter * m_painter;
    gloperate::ResourceManager & m_resourceManager;


private:
    static std::set<Window*> s_instances;
};


} // namespace gloperate_glfw
