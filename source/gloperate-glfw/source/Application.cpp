
#include <gloperate-glfw/Application.h>

#include <cassert>

#ifdef WIN32
#include <Windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/WindowEventDispatcher.h>


namespace
{
#ifdef WIN32
    const std::string g_sep = "\\";
#elif __APPLE__
    const std::string g_sep = "/";
#else
    const std::string g_sep = "/";
#endif
}


namespace gloperate_glfw
{


Application * Application::s_app(nullptr);


Application::Application(int & argc, char ** argv)
: m_exitCode(0)
, m_running(false)
{
    assert(!s_app);
    s_app = this;

    // Determine application path (path to executable)
    #ifdef WIN32
        // On Windows, it can be queried by using to module handle to the application
        HMODULE appModule = GetModuleHandle(0);

        char szFilename[MAX_PATH];

        if (GetModuleFileNameA(appModule, szFilename, MAX_PATH) > 0) 
        {
            _splitpath(szFilename, NULL, NULL, NULL, NULL);
            m_filePath = std::string(szFilename);
            m_path = path(m_filePath);
            m_name = baseName(m_filePath);
        }
    #else
        // Given by the first argument on *nix system
        if (argc > 0)
        {
            m_filePath = argv[0];
            m_path = path(m_filePath);
            m_name = baseName(m_filePath);
        }
    #endif
}

const std::string & Application::applicationFilePath()
{
    assert(s_app);
    return s_app->m_filePath;
}

const std::string & Application::applicationPath()
{
    assert(s_app);
    return s_app->m_path;
}

void Application::setApplicationName(const std::string & name)
{
    assert(s_app);
    s_app->m_name = name;
}

const std::string & Application::applicationName()
{
    assert(s_app);
    return s_app->m_name;
}

int Application::exec()
{
    assert(s_app);

    if (s_app->isRunning())
        return 1;

    s_app->start();

    return s_app->exitCode();
}

void Application::quit(int code)
{
    assert(s_app);

    s_app->stop(code);
}

void Application::start()
{
    m_running = true;

    WindowEventDispatcher::initializeTime();

    while (m_running)
    {
        pollEvents();
        processEvents();
    };

    glfwTerminate();
}

void Application::stop(int code)
{
    m_exitCode = code;
    m_running = false;
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
    WindowEventDispatcher::checkForTimerEvents();
}

void Application::processEvents()
{
    for (Window * window : Window::instances())
    {
        if (window->hasPendingEvents())
            window->processEvents();
        else
            window->idle();
    }
}

std::string Application::baseName(const std::string & filePath)
{
    auto i = filePath.find_last_of(g_sep);

    std::string fileName = filePath;

    if (i == std::string::npos)
    {
        fileName = filePath.substr(++i);
    }

    while (fileName.length() > 0 && fileName[0] == '.') // note: filename might start with '.'
        fileName = fileName.substr(1);

    i = fileName.find_last_not_of(".");

    return i == fileName.npos ? fileName : fileName.substr(--i);
}

std::string Application::path(const std::string & filePath)
{
    auto i = filePath.find_last_of(g_sep);
    return i == filePath.npos ? "" : filePath.substr(0, i);
}


} // namespace gloperate_glfw
