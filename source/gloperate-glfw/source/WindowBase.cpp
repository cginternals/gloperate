
#include <gloperate-glfw/WindowBase.h>

#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/base/baselogging.h>

#include <gloperate/viewer/ContextFormat.h>

#include <gloperate-glfw/Application.h>
#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/WindowEventHandlerBase.h>
#include <gloperate-glfw/WindowEventDispatcher2.h>
#include <gloperate-glfw/WindowEvent.h>


using namespace gloperate;


namespace gloperate_glfw
{


std::set<WindowBase*> WindowBase::s_instances;


const std::set<WindowBase*> & WindowBase::instances()
{
    return s_instances;
}


WindowBase::WindowBase()
: m_window(nullptr)
, m_context(nullptr)
, m_windowMode(WindowedMode)
, m_windowedModeSize(0, 0)
, m_quitOnDestroy(true)
{
    s_instances.insert(this);
}

WindowBase::~WindowBase()
{
    s_instances.erase(this);

    if (m_context)
    {
        finalizeEventHandler();
        WindowEventDispatcher2::deregisterWindow(this);
        destroyContext();
    }

    if (s_instances.empty())
    {
        Application::quit(0);
    }
}

bool WindowBase::create(const ContextFormat & format, const std::string & title, int width, int height)
{
    if (!create(format, width, height))
    {
        return false;
    }

    setTitle(title);
    return true;
}

bool WindowBase::create(const ContextFormat & format, int width, int height)
{
    assert(nullptr == m_context);
    if (m_context)
    {
        return false;
    }

    if (!createContext(format, width, height))
    {
        globjects::fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }

    WindowEventDispatcher2::registerWindow(this);
    initializeEventHandler();

    m_windowedModeSize = glm::ivec2(width, height);

    return true;
}

void WindowBase::destroy()
{
    finalizeEventHandler();
    destroyContext();

    if (m_quitOnDestroy)
    {
        Application::quit(0);
    }
}

GLFWwindow * WindowBase::internalWindow() const
{
    return m_window;
}

Context * WindowBase::context() const
{
    return m_context;
}

void WindowBase::show()
{
    if (!m_window)
    {
        return;
    }

    glfwShowWindow(m_window);
}

void WindowBase::hide()
{
    if (!m_window)
    {
        return;
    }

    glfwHideWindow(m_window);
}

void WindowBase::close()
{
    queueEvent(new CloseEvent);
}

bool WindowBase::isFullscreen() const
{
    return (m_windowMode == FullscreenMode);
}

void WindowBase::setFullscreen(bool fullscreen)
{
    // Switch to fullscreen-mode
    if (fullscreen && m_windowMode != FullscreenMode)
    {
        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        if (!monitor)
        {
            return;
        }

        m_windowedModeSize = size();

        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        int w = mode->width;
        int h = mode->height;

        ContextFormat format = m_context->format();

        finalizeEventHandler();
        WindowEventDispatcher2::deregisterWindow(this);
        destroyContext();

        if (createContext(format, w, h, monitor))
        {
            WindowEventDispatcher2::registerWindow(this);
            initializeEventHandler();

            m_windowMode = FullscreenMode;
        }
    }

    // Switch to windowed-mode
    else if (!fullscreen && m_windowMode != WindowedMode)
    {
        int w = m_windowedModeSize.x;
        int h = m_windowedModeSize.y;

        ContextFormat format = m_context->format();

        finalizeEventHandler();
        WindowEventDispatcher2::deregisterWindow(this);
        destroyContext();

        if (createContext(format, w, h, nullptr))
        {
            WindowEventDispatcher2::registerWindow(this);
            initializeEventHandler();

            m_windowMode = WindowedMode;
        }
    }
}

glm::ivec2 WindowBase::position() const
{
    if (!m_window)
    {
        return glm::ivec2();
    }

    int x, y;
    glfwGetWindowPos(m_window, &x, &y);

    return glm::ivec2(x, y);
}

glm::ivec2 WindowBase::size() const
{
    if (!m_window)
    {
        return glm::ivec2();
    }

    int w, h;
    glfwGetWindowSize(m_window, &w, &h);

    return glm::ivec2(w, h);
}

void WindowBase::resize(int width, int height)
{
    if (!m_window)
    {
        return;
    }

    glfwSetWindowSize(m_window, width, height);
}

glm::ivec2 WindowBase::framebufferSize() const
{
    if (!m_window)
    {
        return glm::ivec2();
    }

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    return glm::ivec2(w, h);
}

const std::string & WindowBase::title() const
{
    return m_title;
}

void WindowBase::setTitle(const std::string & title)
{
    if (!m_window)
    {
        return;
    }

    glfwSetWindowTitle(m_window, title.c_str());

    m_title = title;
}

int WindowBase::inputMode(int mode) const
{
    if (!m_window)
    {
        return;
    }

    return glfwGetInputMode(m_window, mode);
}

void WindowBase::setInputMode(int mode, int value)
{
    if (!m_window)
    {
        return;
    }

    glfwSetInputMode(m_window, mode, value);
}

bool WindowBase::quitsOnDestroy() const
{
    return m_quitOnDestroy;
}

void WindowBase::setQuitOnDestroy(bool quitOnDestroy)
{
    m_quitOnDestroy = quitOnDestroy;
}

void WindowBase::repaint()
{
    queueEvent(new PaintEvent);
}

void WindowBase::swap()
{
    glfwSwapBuffers(m_window);
}

void WindowBase::addTimer(int id, int interval, bool singleShot)
{
    WindowEventDispatcher2::addTimer(this, id, interval, singleShot);
}

void WindowBase::removeTimer(int id)
{
    WindowEventDispatcher2::removeTimer(this, id);
}

void WindowBase::queueEvent(WindowEvent * event)
{
    if (!event)
    {
        return;
    }

    m_eventQueue.push(event);
}

bool WindowBase::hasPendingEvents()
{
    return !m_eventQueue.empty();
}

void WindowBase::processEvents()
{
    if (m_eventQueue.empty() || !m_context)
    {
        return;
    }

    glfwMakeContextCurrent(m_window);

    while (!m_eventQueue.empty())
    {
        WindowEvent * event = m_eventQueue.front();
        m_eventQueue.pop();
        event->setWindow(this);

        processEvent(*event);

        delete event;

        if (!m_context)
        {
            clearEventQueue();
            return;
        }
    }

    glfwMakeContextCurrent(nullptr);
}

bool WindowBase::createContext(const ContextFormat & format, int width, int height, GLFWmonitor * /*monitor*/)
{
    assert(nullptr == m_context);
    if (m_context)
    {
        return false;
    }

    m_window = Context::create(format);
    if (!m_window)
    {
        return false;
    }

    glfwSetWindowSize(m_window, width, height);

    m_context = new Context(m_window);
    m_context->format().verify(format);

    return true;
}

void WindowBase::destroyContext()
{
    delete m_context;
    glfwDestroyWindow(m_window);

    m_context = nullptr;
    m_window = nullptr;
}

void WindowBase::initializeEventHandler()
{
    if (!m_eventHandler)
    {
        return;
    }

    glfwMakeContextCurrent(m_window);
    m_eventHandler->initialize(*this);
    glfwMakeContextCurrent(nullptr);

    queueEvent(new ResizeEvent(size()));
    queueEvent(new ResizeEvent(framebufferSize(), true));
}

void WindowBase::finalizeEventHandler()
{
    if (m_eventHandler)
    {
        glfwMakeContextCurrent(m_window);
        m_eventHandler->finalize(*this);
        glfwMakeContextCurrent(nullptr);
    }
}

void WindowBase::setEventHandler(WindowEventHandlerBase * eventHandler)
{
    if (eventHandler == m_eventHandler)
    {
        return;
    }

    m_eventHandler = eventHandler;

    if (!m_eventHandler)
    {
        return;
    }

    if (!m_context)
    {
        return;
    }

    initializeEventHandler();
}

void WindowBase::clearEventQueue()
{
    while (!m_eventQueue.empty())
    {
        delete m_eventQueue.front();
        m_eventQueue.pop();
    }
}

void WindowBase::processEvent(WindowEvent & event)
{
    if (m_eventHandler)
    {
        m_eventHandler->handleEvent(event);
    }

    postprocessEvent(event);
}

void WindowBase::postprocessEvent(WindowEvent & event)
{
    switch (event.type())
    {
        case WindowEvent::Type::Paint:
            swap();
            break;

        case WindowEvent::Type::Close:
            if (!event.isAccepted())
            {
                destroy();
            }
            break;

        default:
            break;
    }
}

void WindowBase::idle()
{
    if (m_eventHandler)
    {
        m_eventHandler->idle(*this);
    }
}


} // namespace gloperate_glfw
