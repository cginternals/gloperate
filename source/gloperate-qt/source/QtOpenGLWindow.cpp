#include "gloperate-qt/QtOpenGLWindow.h"
#include <gloperate-qt/qt-includes-begin.h>
#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QOpenGLContext>
#include <gloperate-qt/qt-includes-end.h>


namespace gloperate_qt
{


QSurfaceFormat QtOpenGLWindow::defaultFormat()
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    #ifndef NDEBUG
        format.setOption(QSurfaceFormat::DebugContext);
    #endif
    return format;
}

QtOpenGLWindow::QtOpenGLWindow()
: QtOpenGLWindow(QtOpenGLWindow::defaultFormat())
{
}

QtOpenGLWindow::QtOpenGLWindow(const QSurfaceFormat& format)
: m_context(new QOpenGLContext)
, m_initialized(false)
, m_updatePending(false)
{
    QSurfaceFormat f(format);
    f.setRenderableType(QSurfaceFormat::OpenGL);

    setSurfaceType(OpenGLSurface);
    create();

    m_context->setFormat(format);
    if (!m_context->create()) {
        qDebug() << "Could not create OpenGL context.";
        QApplication::quit();
    } else {
        qDebug().nospace() << "Created OpenGL context " << m_context->format().version().first << "." << m_context->format().version().second;
    }
}

QtOpenGLWindow::~QtOpenGLWindow()
{
}

QOpenGLContext *QtOpenGLWindow::context() const
{
    return m_context.data();
}

void QtOpenGLWindow::updateGL()
{
    if (!m_updatePending) {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void QtOpenGLWindow::initialize()
{
    m_context->makeCurrent(this);

    onInitialize();

    m_context->doneCurrent();

    m_initialized = true;
}

void QtOpenGLWindow::resize(QResizeEvent * event)
{
    if (!m_initialized) {
        initialize();
    }

    m_context->makeCurrent(this);

    QResizeEvent deviceSpecificResizeEvent(event->size() * devicePixelRatio(), event->oldSize() * devicePixelRatio());

    onResize(&deviceSpecificResizeEvent);

    m_context->doneCurrent();
}

void QtOpenGLWindow::paint()
{
    if (!m_initialized) {
        initialize();
    }

    if (!isExposed()) {
        return;
    }

    m_updatePending = false;

    m_context->makeCurrent(this);

    onPaint();

    m_context->swapBuffers(this);

    m_context->doneCurrent();
}

void QtOpenGLWindow::onInitialize()
{
}

void QtOpenGLWindow::onResize(QResizeEvent * )
{
}

void QtOpenGLWindow::onPaint()
{
}

bool QtOpenGLWindow::event(QEvent * event)
{
    switch (event->type()) {
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

void QtOpenGLWindow::resizeEvent(QResizeEvent * event)
{
    resize(event);
    paint();
}

void QtOpenGLWindow::exposeEvent(QExposeEvent * )
{
    paint();
}

void QtOpenGLWindow::enterEvent(QEvent *)
{
}

void QtOpenGLWindow::leaveEvent(QEvent *)
{
}


} // namespace gloperate-qt
