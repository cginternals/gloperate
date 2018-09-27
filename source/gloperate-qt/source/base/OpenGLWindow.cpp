
#include "gloperate-qt/base/OpenGLWindow.h"

#include <QCoreApplication>
#include <QResizeEvent>
#include <QOpenGLContext>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/GLContextFactory.h>


namespace gloperate_qt
{


OpenGLWindow::OpenGLWindow()
: m_context(nullptr)
, m_initialized(false)
, m_updatePending(false)
{
    // Connect update timer
    QObject::connect(
        &m_timer, &QTimer::timeout,
        this, &OpenGLWindow::onTimer
    );

    // Create window with OpenGL capability
    setSurfaceType(OpenGLSurface);
    create();

    // Start timer
    m_timer.setSingleShot(false);
    m_timer.start(5);
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::setContextFormat(const gloperate::GLContextFormat & format)
{
    m_format = format;
}

GLContext * OpenGLWindow::context() const
{
    return m_context.get();
}

void OpenGLWindow::createContext()
{
    // Destroy old context
    if (m_context)
    {
        deinitializeContext();
    }

    // Create OpenGL context
    GLContextFactory factory(this);
    m_context = std::unique_ptr<GLContext>(
        static_cast<GLContext*>(factory.createBestContext(m_format).release())
    );

    // Initialize new context
    initializeContext();
}

void OpenGLWindow::destroyContext()
{
    // Destroy old context
    if (!m_context)
    {
        return;
    }

    deinitializeContext();

    m_context = nullptr;
}

void OpenGLWindow::updateGL()
{
    if (!m_updatePending)
    {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void OpenGLWindow::initializeContext()
{
    if (m_initialized)
    {
        return;
    }

    m_context->qtContext()->makeCurrent(this);
    onContextInit();
    m_context->qtContext()->doneCurrent();

    m_initialized = true;
}

void OpenGLWindow::deinitializeContext()
{
    if (!m_initialized)
    {
        return;
    }

    m_context->qtContext()->makeCurrent(this);
    onContextDeinit();
    m_context->qtContext()->doneCurrent();

    m_initialized = false;
}

void OpenGLWindow::resize(QResizeEvent * event)
{
    if (!m_context)
    {
        return;
    }

    if (!m_initialized) {
        initializeContext();
    }

    m_context->qtContext()->makeCurrent(this);

    QSize deviceSize  = event->size() * devicePixelRatio();
    QSize virtualSize = event->size();
    onResize(deviceSize, virtualSize);

    m_context->qtContext()->doneCurrent();
}

void OpenGLWindow::paint()
{
    if (!m_context)
    {
        return;
    }

    if (!m_initialized) {
        initializeContext();
    }

    if (!isExposed()) {
        return;
    }

    m_updatePending = false;

    m_context->qtContext()->makeCurrent(this);
    onPaint();
    m_context->qtContext()->swapBuffers(this);
    m_context->qtContext()->doneCurrent();
}

void OpenGLWindow::onContextInit()
{
}

void OpenGLWindow::onContextDeinit()
{
}

void OpenGLWindow::onResize(const QSize &, const QSize &)
{
}

void OpenGLWindow::onPaint()
{
}

void OpenGLWindow::onTimer()
{
}

bool OpenGLWindow::event(QEvent * event)
{
    switch (event->type())
    {
        case QEvent::UpdateRequest:
            paint();
            return true;

        case QEvent::Enter:
            enterEvent(event);
            return true;

        case QEvent::Leave:
            leaveEvent(event);
            return true;

        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::resizeEvent(QResizeEvent * event)
{
    resize(event);
}

void OpenGLWindow::exposeEvent(QExposeEvent * )
{
    paint();
}

void OpenGLWindow::enterEvent(QEvent *)
{
}

void OpenGLWindow::leaveEvent(QEvent *)
{
}


} // namespace gloperate_qt
