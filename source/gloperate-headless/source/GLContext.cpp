
#include <gloperate-headless/GLContext.h>

#include <cassert>

#include <eglbinding/egl/egl.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>

#include <gloperate-headless/getProcAddress.h>


using namespace gloperate;
using namespace egl;


namespace gloperate_headless
{


void GLContext::updateSwapBehavior(gloperate::GLContextFormat::SwapBehavior swapBehavior)
{
    switch (swapBehavior)
    {
    case gloperate::GLContextFormat::SwapBehavior::DoubleBuffering:
        // TODO:
        //glfwSwapInterval(1);
        break;

    default:
        // TODO:
        //glfwSwapInterval(0);
        break;
    }
}

GLContext::GLContext(EGLDisplay display, EGLSurface surface, EGLContext context)
: m_display(display)
, m_surface(surface)
, m_context(context)
{
    // Activate context
    use();

    // Initialize glbinding and globjects in context (needed for context utils)
    initializeBindings([](const char * name) -> glbinding::ProcAddress
    {
        return getProcAddress(name);
    });

    // Read context format
    m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    release();
}

GLContext::~GLContext()
{
}

egl::EGLSurface GLContext::surface() const
{
    return m_surface;
}

egl::EGLContext GLContext::context() const
{
    return m_context;
}

void GLContext::use() const
{
    if (m_display && m_surface && m_context)
    {
        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
    }
}

void GLContext::release() const
{
    if (m_display)
    {
        eglMakeCurrent(m_display, nullptr, nullptr, nullptr);
    }
}


} // namespace gloperate_headless
