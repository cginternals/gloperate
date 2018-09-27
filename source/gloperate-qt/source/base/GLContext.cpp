
#include <gloperate-qt/base/GLContext.h>

#include <cassert>

#include <gloperate/base/GLContextUtils.h>

#include <gloperate-qt/base/QtOpenGL.h>


using namespace gloperate;


namespace gloperate_qt
{


GLContext::GLContext(QWindow * window, QOpenGLContext * context, bool takeOwnership)
: m_window(window)
, m_context(context)
, m_ownsContext(takeOwnership)
{
    assert(window);
    assert(context);

    // Activate context
    use();

    // Initialize glbinding and globjects in context (needed for context utils)
    initializeBindings([this] (const char * name) -> glbinding::ProcAddress {
        return QtOpenGL::getProcAddress(m_context, name);
    });

    // Read context format
    m_format = GLContextUtils::retrieveFormat();

    // Deactivate context
    release();
}

GLContext::~GLContext()
{
    destroyContext();
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
