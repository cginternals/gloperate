
#include <gloperate-qt/base/GLContext.h>

#include <QWindow>
#include <QOpenGLContext>


namespace gloperate_qt
{


QOpenGLContext * GLContext::qtContext() const
{
    return m_context;
}

void GLContext::destroyContext()
{
    if (m_ownsContext)
    {
        delete m_context;
    }

    m_context = nullptr;
}

void GLContext::makeCurrent(QOpenGLContext * context, QWindow * window)
{
    context->makeCurrent(window);
}

void GLContext::doneCurrent(QOpenGLContext * context)
{
    context->doneCurrent();
}


} // namespace gloperate_qt
