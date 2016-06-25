
#include <gloperate-glfw/Window.h>

#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cppassist/logging/logging.h>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-glfw/GLContext.h>
#include <gloperate-glfw/GLContextFactory.h>
#include <gloperate-glfw/Application.h>
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
, m_windowedModeSize(100, 100)
, m_quitOnDestroy(true)
, m_title("")
, m_format()
, m_needsRepaint(false)
{
    // Register window
    s_instances.insert(this);
}

Window::~Window()
{
    // Unregister window
    s_instances.erase(this);

    // Destroy window
    destroy();
}

bool Window::setContextFormat(const gloperate::GLContextFormat & format)
{
    // If window has already been created, the context format cannot be changed anymore
    if (m_context != nullptr)
    {
        return false;
    }

    // Save new context format
    m_format = format;
    return true;
}

bool Window::create()
{
    // Abort, if the window has already been created
    if (m_context)
    {
        return false;
    }

    // Create internal window
    if (!createInternalWindow(m_format, m_windowedModeSize.x, m_windowedModeSize.y))
    {
        cppassist::fatal() << "Creating native window with OpenGL context failed.";
        return false;
    }

    return true;
}

void Window::destroy()
{
    // Destroy internal window
    if (m_context)
    {
        destroyInternalWindow();
    }

    // Quit application?
    if (m_quitOnDestroy)
    {
        Application::quit(0);
    }
}

GLContext * Window::context() const
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
    if (!m_window)
    {
        return;
    }

    queueEvent(new CloseEvent);
}

bool Window::isFullscreen() const
{
    if (!m_window)
    {
        return false;
    }

    return (m_windowMode == FullscreenMode);
}

void Window::setFullscreen(bool fullscreen)
{
    if (!m_window)
    {
        return;
    }

    // Switch to fullscreen-mode
    if (fullscreen && m_windowMode != FullscreenMode)
    {
        // Get monitor on which fullscreen mode is requested
        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        if (!monitor)
        {
            return;
        }

        // Remember old window size
        m_windowedModeSize = size();

        // Set window size to monitor size
        const GLFWvidmode * mode = glfwGetVideoMode(monitor);
        int w = mode->width;
        int h = mode->height;

        // Destroy internal window
        destroyInternalWindow();

        // Create new internal window
        if (createInternalWindow(m_format, w, h, monitor))
        {
            // Save fullscreen mode
            m_windowMode = FullscreenMode;
        }
    }

    // Switch to windowed-mode
    else if (!fullscreen && m_windowMode != WindowedMode)
    {
        int w = m_windowedModeSize.x;
        int h = m_windowedModeSize.y;

        // Destroy internal window
        destroyInternalWindow();

        // Create new internal window
        if (createInternalWindow(m_format, w, h, nullptr))
        {
            // Save windowed mode
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

void Window::setSize(int width, int height)
{
    m_windowedModeSize = glm::ivec2(width, height);

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
    m_title = title;

    if (!m_window)
    {
        return;
    }

    glfwSetWindowTitle(m_window, title.c_str());
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
    if (!m_window)
    {
        return;
    }

    m_needsRepaint = true;

    Application::wakeup();
}

void Window::swap()
{
    if (!m_window)
    {
        return;
    }

    glfwSwapBuffers(m_window);
}

GLFWwindow * Window::internalWindow() const
{
    return m_window;
}

void Window::queueEvent(WindowEvent * event)
{
    if (!event)
    {
        return;
    }

    m_eventQueue.push(event);
}

void Window::updateRepaintEvent()
{
    if (m_needsRepaint)
    {
        m_needsRepaint = false;

        queueEvent(new PaintEvent);
    }
}

bool Window::hasPendingEvents()
{
    if (!m_window)
    {
        return false;
    }

    return !m_eventQueue.empty();
}

void Window::processEvents()
{
    if (m_eventQueue.empty() || !m_window)
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

bool Window::createInternalWindow(const GLContextFormat & format, int width, int height, GLFWmonitor * /*monitor*/)
{
    // Abort if window is already created
    assert(nullptr == m_context);
    if (m_context)
    {
        return false;
    }

    // Create GLFW window with OpenGL context
    GLContextFactory factory;
    m_context = static_cast<GLContext*>(factory.createBestContext(format));
    if (!m_context)
    {
        return false;
    }

    // Check OpenGL format
    m_context->format().verify(format);

    // Get internal window
    m_window = m_context->window();

    // Set window size and title
    glfwSetWindowSize (m_window, width, height);
    glfwSetWindowTitle(m_window, m_title.c_str());

    // Register window for event processing
    WindowEventDispatcher::registerWindow(this);

    // Initialize rendering components with new context
    glfwMakeContextCurrent(m_window);
    onContextInit();
    glfwMakeContextCurrent(nullptr);

    // Promote current size
    queueEvent(new ResizeEvent(size()));
    queueEvent(new ResizeEvent(framebufferSize(), true));

    return true;
}

void Window::destroyInternalWindow()
{
    // Abort if window has not been created
    if (!m_context)
    {
        return;
    }

    // Deinitialize rendering components from old context
    glfwMakeContextCurrent(m_window);
    onContextDeinit();
    glfwMakeContextCurrent(nullptr);

    // Unregister window from event processing
    WindowEventDispatcher::deregisterWindow(this);

    // Destroy GLFW window and OpenGL context
    delete m_context;
    glfwDestroyWindow(m_window);

    // Reset internal pointers
    m_context = nullptr;
    m_window = nullptr;
}

void Window::onContextInit()
{
}

void Window::onContextDeinit()
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


} // namespace gloperate_glfw
