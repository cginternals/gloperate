
#include <gloperate-qt/base/GLContext.h>

#include <QSurface>
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

void GLContext::makeCurrent(QOpenGLContext * context, QSurface * surface)
{
    context->makeCurrent(surface);
}

void GLContext::doneCurrent(QOpenGLContext * context)
{
    context->doneCurrent();
}


} // namespace gloperate_qt
