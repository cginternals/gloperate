
#include <gloperate-headless/Application.h>

#include <cassert>
#include <chrono>
#include <thread>

#include <eglbinding/eglbinding.h>
#include <eglbinding/egl/egl.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/TimerManager.h>

#include <gloperate-headless/Surface.h>
#include <gloperate-headless/getProcAddress.h>


using namespace egl;


namespace gloperate_headless
{


Application * Application::s_app = nullptr;


int Application::init()
{
    eglbinding::initialize(getProcAddress);

    return true;
}

void Application::quit(int code)
{
    // An application needs to be initialized already
    assert(s_app);

    // Stop application
    s_app->stop(code);
}

void Application::wakeup()
{
    // TODO: empty?
}

Application::Application(gloperate::Environment * environment, int &, char **)
: m_environment(environment)
, m_running(false)
, m_exitCode(0)
, m_display(nullptr)
{
    // Make sure that no application object has already been instanciated
    assert(!s_app);
    s_app = this;

    // Connect to exit-signal
    environment->exitApplication.connect([] (int exitCode)
    {
        Application::quit(exitCode);
    });

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    assert(m_display != nullptr);

    EGLint vmajor, vminor;

    const auto result = eglInitialize(m_display, &vmajor, &vminor);
    (void)result;
    assert(result);
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

    // Execute main loop
    while (m_running)
    {
        // Wait until drawing finished.
        eglWaitClient();
        processEvents();
    }

    // Deinitialize eglbinding
    eglTerminate(m_display);

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

void Application::processEvents()
{
    // Get messages for all surfaces
    for (Surface * surface : Surface::instances())
    {
        // Update timing
        surface->idle();

        // If surface needs updating, let it send an udate event
        surface->updateRepaintEvent();

        // Process all events for the surface
        if (surface->hasPendingEvents()) {
            surface->processEvents();
        }
    }

    // Update scripting timers
    m_environment->timerManager()->update();

    // Make sure we don't saturate the CPU 
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    // Wake up mainloop to enable continuous update/simulation
    wakeup();
}


} // namespace gloperate_headless
