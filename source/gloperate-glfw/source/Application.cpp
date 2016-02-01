
#include <gloperate-glfw/Application.h>

#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gloperate-glfw/WindowBase.h>
#include <gloperate-glfw/WindowEventDispatcher2.h>


namespace gloperate_glfw
{


Application * Application::s_app = nullptr;


int Application::init()
{
    return glfwInit();
}

void Application::quit(int code)
{
    // An application needs to be initialized already
    assert(s_app);

    // Stop application
    s_app->stop(code);
}

Application::Application(int &, char **)
: m_running(false)
, m_exitCode(0)
{
    // Make sure that no application object has already been instanciated
    assert(!s_app);
    s_app = this;
}

Application::~Application()
{
    // Deregister application
    s_app = nullptr;
}

int Application::run()
{
    // Abort if application is already running
    if (m_running)
    {
        return 1;
    }

    // Start application
    m_running  = true;
    m_exitCode = 0;
    WindowEventDispatcher2::initializeTime();

    // Execute main loop
    while (m_running)
    {
        pollEvents();
        processEvents();
    }

    // Deinitialize GLFW
    glfwTerminate();

    // Return with exit code
    return m_exitCode;
}

void Application::stop(int code)
{
    // Stop application, return the given exit code
    m_exitCode = code;
    m_running  = false;
}

bool Application::isRunning() const
{
    return m_running;
}

int Application::exitCode()
{
    return m_exitCode;
}

void Application::pollEvents()
{
    glfwPollEvents();
    WindowEventDispatcher2::checkForTimerEvents();
}

void Application::processEvents()
{
    // Get messages for all windows
    for (WindowBase * window : WindowBase::instances())
    {
        if (window->hasPendingEvents()) {
            window->processEvents();
        } else {
            window->idle();
        }
    }
}


} // namespace gloperate_glfw
