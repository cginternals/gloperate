
#include <gloperate-qt/viewer/GLContext.h>

#include <QOpenGLContext>

#include <gloperate-qt/viewer/OpenGLWindow.h>


namespace gloperate_qt
{


QOpenGLContext * GLContext::qtContext() const
{
    return m_context;
}

void GLContext::destroyContext()
{
    delete m_context;
}

void GLContext::makeCurrent(QOpenGLContext * context, OpenGLWindow * window)
{
    context->makeCurrent(window);
}

void GLContext::doneCurrent(QOpenGLContext * context)
{
    context->doneCurrent();
}


} // namespace gloperate_qt
