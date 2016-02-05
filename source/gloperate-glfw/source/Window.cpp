
#include <gloperate-glfw/Window.h>

#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/base/baselogging.h>

#include <gloperate/base/ContextFormat.h>

#include <gloperate-glfw/Application.h>
#include <gloperate-glfw/Context.h>
#include <gloperate-glfw/WindowEventDispatcher.h>
#include <gloperate-glfw/WindowEvent.h>


using namespace gloperate;


namespace gloperate_glfw
{


std::set<Window*> Window::s_instances;


const std::set<Window*> & Window::instances()
{
    return s_instances;
}


Window::Window()
: m_window(nullptr)
, m_context(nullptr)
, m_windowMode(WindowedMode)
, m_windowedModeSize(0, 0)
, m_quitOnDestroy(true)
{
    s_instances.insert(this);
}

Window::~Window()
{
    s_instances.erase(this);

    if (m_context)
    {
        deinitializeContext();
        WindowEventDispatcher::deregisterWindow(this);
        destroyContext();
    }

    if (s_instances.empty())
    {
        Application::quit(0);
    }
}

bool Window::create(const ContextFormat & format, const std::string & title, int width, int height)
{
    if (!create(format, width, height))
    {
        return false;
    }

    setTitle(title);
    return true;
}

bool Window::create(const ContextFormat & format, int width, int height)
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

    WindowEventDispatcher::registerWindow(this);
    initializeContext();

    m_windowedModeSize = glm::ivec2(width, height);

    return true;
}

void Window::destroy()
{
    deinitializeContext();
    destroyContext();

    if (m_quitOnDestroy)
    {
        Application::quit(0);
    }
}

GLFWwindow * Window::internalWindow() const
{
    return m_window;
}

Context * Window::context() const
{
    return m_context;
}

void Window::show()
{
    if (!m_window)
    {
        return;
    }

    glfwShowWindow(m_window);
}

void Window::hide()
{
    if (!m_window)
    {
        return;
    }

    glfwHideWindow(m_window);
}

void Window::close()
{
    queueEvent(new CloseEvent);
}

bool Window::isFullscreen() const
{
    return (m_windowMode == FullscreenMode);
}

void Window::setFullscreen(bool fullscreen)
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

        deinitializeContext();
        WindowEventDispatcher::deregisterWindow(this);
        destroyContext();

        if (createContext(format, w, h, monitor))
        {
            WindowEventDispatcher::registerWindow(this);
            initializeContext();

            m_windowMode = FullscreenMode;
        }
    }

    // Switch to windowed-mode
    else if (!fullscreen && m_windowMode != WindowedMode)
    {
        int w = m_windowedModeSize.x;
        int h = m_windowedModeSize.y;

        ContextFormat format = m_context->format();

        deinitializeContext();
        WindowEventDispatcher::deregisterWindow(this);
        destroyContext();

        if (createContext(format, w, h, nullptr))
        {
            WindowEventDispatcher::registerWindow(this);
            initializeContext();

            m_windowMode = WindowedMode;
        }
    }
}

glm::ivec2 Window::position() const
{
    if (!m_window)
    {
        return glm::ivec2();
    }

    int x, y;
    glfwGetWindowPos(m_window, &x, &y);

    return glm::ivec2(x, y);
}

glm::ivec2 Window::size() const
{
    if (!m_window)
    {
        return glm::ivec2();
    }

    int w, h;
    glfwGetWindowSize(m_window, &w, &h);

    return glm::ivec2(w, h);
}

void Window::resize(int width, int height)
{
    if (!m_window)
    {
        return;
    }

    glfwSetWindowSize(m_window, width, height);
}

glm::ivec2 Window::framebufferSize() const
{
    if (!m_window)
    {
        return glm::ivec2();
    }

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    return glm::ivec2(w, h);
}

const std::string & Window::title() const
{
    return m_title;
}

void Window::setTitle(const std::string & title)
{
    if (!m_window)
    {
        return;
    }

    glfwSetWindowTitle(m_window, title.c_str());

    m_title = title;
}

int Window::inputMode(int mode) const
{
    if (!m_window)
    {
        return 0;
    }

    return glfwGetInputMode(m_window, mode);
}

void Window::setInputMode(int mode, int value)
{
    if (!m_window)
    {
        return;
    }

    glfwSetInputMode(m_window, mode, value);
}

bool Window::quitsOnDestroy() const
{
    return m_quitOnDestroy;
}

void Window::setQuitOnDestroy(bool quitOnDestroy)
{
    m_quitOnDestroy = quitOnDestroy;
}

void Window::repaint()
{
    queueEvent(new PaintEvent);
}

void Window::swap()
{
    glfwSwapBuffers(m_window);
}

void Window::addTimer(int id, int interval, bool singleShot)
{
    WindowEventDispatcher::addTimer(this, id, interval, singleShot);
}

void Window::removeTimer(int id)
{
    WindowEventDispatcher::removeTimer(this, id);
}

void Window::queueEvent(WindowEvent * event)
{
    if (!event)
    {
        return;
    }

    m_eventQueue.push(event);
}

bool Window::hasPendingEvents()
{
    return !m_eventQueue.empty();
}

void Window::processEvents()
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

        handleEvent(*event);

        delete event;

        if (!m_context)
        {
            clearEventQueue();
            return;
        }
    }

    glfwMakeContextCurrent(nullptr);
}

void Window::handleEvent(WindowEvent & event)
{
    // Check that event is meant for this window
    if (event.window() != this)
    {
        return;
    }

    // Dispatch event
    switch (event.type())
    {
        case WindowEvent::Type::Resize:
            onResize(static_cast<ResizeEvent &>(event));
            break;

        case WindowEvent::Type::FrameBufferResize:
            onFramebufferResize(static_cast<ResizeEvent &>(event));
            break;

        case WindowEvent::Type::Move:
            onMove(static_cast<MoveEvent &>(event));
            break;

        case WindowEvent::Type::Paint:
            onPaint(static_cast<PaintEvent &>(event));
            swap();
            break;

        case WindowEvent::Type::KeyPress:
            onKeyPress(static_cast<KeyEvent &>(event));
            break;

        case WindowEvent::Type::KeyRelease:
            onKeyRelease(static_cast<KeyEvent &>(event));
            break;

        case WindowEvent::Type::MousePress:
            onMousePress(static_cast<MouseEvent &>(event));
            break;

        case WindowEvent::Type::MouseRelease:
            onMouseRelease(static_cast<MouseEvent &>(event));
            break;

        case WindowEvent::Type::MouseMove:
            onMouseMove(static_cast<MouseEvent &>(event));
            break;

        case WindowEvent::Type::MouseEnter:
            onMouseEnter(static_cast<MouseEnterEvent &>(event));
            break;

        case WindowEvent::Type::MouseLeave:
            onMouseLeave(static_cast<MouseLeaveEvent &>(event));
            break;

        case WindowEvent::Type::Scroll:
            onScroll(static_cast<ScrollEvent &>(event));
            break;

        case WindowEvent::Type::Focus:
            onFocus(static_cast<FocusEvent &>(event));
            break;

        case WindowEvent::Type::Iconify:
            onIconify(static_cast<IconifyEvent &>(event));
            break;

        case WindowEvent::Type::Timer:
            onTimer(static_cast<TimerEvent &>(event));
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

void Window::clearEventQueue()
{
    while (!m_eventQueue.empty())
    {
        delete m_eventQueue.front();
        m_eventQueue.pop();
    }
}

bool Window::createContext(const ContextFormat & format, int width, int height, GLFWmonitor * /*monitor*/)
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

void Window::destroyContext()
{
    delete m_context;
    glfwDestroyWindow(m_window);

    m_context = nullptr;
    m_window = nullptr;
}

void Window::initializeContext()
{
    glfwMakeContextCurrent(m_window);
    onContextInit();
    glfwMakeContextCurrent(nullptr);

    queueEvent(new ResizeEvent(size()));
    queueEvent(new ResizeEvent(framebufferSize(), true));
}

void Window::deinitializeContext()
{
    glfwMakeContextCurrent(m_window);
    onContextDeinit();
    glfwMakeContextCurrent(nullptr);
}

void Window::onContextInit()
{
}

void Window::onContextDeinit()
{
}

void Window::onIdle()
{
}

void Window::onResize(ResizeEvent &)
{
}

void Window::onFramebufferResize(ResizeEvent &)
{
}

void Window::onMove(MoveEvent &)
{
}

void Window::onPaint(PaintEvent &)
{
}

void Window::onKeyPress(KeyEvent &)
{
}

void Window::onKeyRelease(KeyEvent &)
{
}

void Window::onMousePress(MouseEvent &)
{
}

void Window::onMouseMove(MouseEvent &)
{
}

void Window::onMouseRelease(MouseEvent &)
{
}

void Window::onMouseEnter(MouseEnterEvent &)
{
}

void Window::onMouseLeave(MouseLeaveEvent &)
{
}

void Window::onScroll(ScrollEvent &)
{
}

void Window::onFocus(FocusEvent &)
{
}

void Window::onIconify(IconifyEvent &)
{
}

void Window::onTimer(TimerEvent &)
{
}


} // namespace gloperate_glfw
