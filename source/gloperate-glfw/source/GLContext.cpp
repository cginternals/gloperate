
#include <gloperate-glfw/GLContext.h>

#include <cassert>

#ifdef WIN32
    #include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> // specifies APIENTRY, should be after Error.h include,
                        // which requires APIENTRY in windows..

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>


using namespace gloperate;


namespace gloperate_glfw
{


void GLContext::updateSwapBehavior(gloperate::GLContextFormat::SwapBehavior swapBehavior)
{
    switch (swapBehavior)
    {
    case gloperate::GLContextFormat::SwapBehavior::DoubleBuffering:
        glfwSwapInterval(1);
        break;

    default:
        glfwSwapInterval(0);
    }
}

GLContext::GLContext(GLFWwindow * window)
: m_window(window)
{
    assert(window);

    // Activate context
    use();

    // Initialize glbinding in context (needed for context utils)
    initializeGLBinding();

    // Read context handle and format
    m_handle = GLContextUtils::tryFetchHandle();
    m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    release();
}

GLContext::~GLContext()
{
}

GLFWwindow * GLContext::window() const
{
    return m_window;
}

void GLContext::use() const
{
    if (m_window)
    {
        glfwMakeContextCurrent(m_window);
    }
}

void GLContext::release() const
{
    if (m_window && m_window == glfwGetCurrentContext())
    {
        glfwMakeContextCurrent(nullptr);
    }
}


} // namespace gloperate_glfw
