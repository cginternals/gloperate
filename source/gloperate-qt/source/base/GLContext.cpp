
#include <gloperate-qt/base/GLContext.h>

#include <cassert>

#include <gloperate/base/GLContextUtils.h>


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

    // Initialize glbinding in context (needed for context utils)
    initializeBindings();

    // Read context handle and format
    m_handle = GLContextUtils::tryFetchHandle();
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
