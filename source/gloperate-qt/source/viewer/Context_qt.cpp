
#include <gloperate-qt/viewer/Context.h>

#include <QOpenGLContext>

#include <gloperate-qt/viewer/OpenGLWindow.h>


namespace gloperate_qt
{


void Context::makeCurrent(QOpenGLContext * context, OpenGLWindow * window)
{
    context->makeCurrent(window);
}

void Context::doneCurrent(QOpenGLContext * context)
{
    context->doneCurrent();
}


} // namespace gloperate_qt
