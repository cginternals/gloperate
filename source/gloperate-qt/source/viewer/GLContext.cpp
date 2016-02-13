
#include <gloperate-qt/viewer/GLContext.h>

#include <cassert>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>

#include <gloperate-qt/viewer/OpenGLWindow.h>


using namespace gloperate;


namespace gloperate_qt
{


GLContext::GLContext(OpenGLWindow * window, QOpenGLContext * context)
: m_window(window)
, m_context(context)
{
    assert(window);
    assert(context);

    // Activate context
    GLContext::makeCurrent(m_context, m_window);

    // Initialize glbinding in that context
    glbinding::Binding::initialize(false);

    // Read context handle
    m_handle = GLContextUtils::tryFetchHandle();

    // Read context format
    assert(isValid());
    m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    GLContext::doneCurrent(m_context);
}

GLContext::~GLContext()
{
}

void GLContext::use() const
{
    if (m_window && m_context)
    {
        GLContext::makeCurrent(m_context, m_window);
    }
}

void GLContext::release() const
{
    if (m_context)
    {
        GLContext::doneCurrent(m_context);
    }
}


} // namespace gloperate_qt
