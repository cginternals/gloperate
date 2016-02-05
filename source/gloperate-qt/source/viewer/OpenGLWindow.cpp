
#include "gloperate-qt/viewer/OpenGLWindow.h"

#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QOpenGLContext>

#include <gloperate/base/ContextFormat.h>

#include <gloperate-qt/viewer/Context.h>


namespace gloperate_qt
{


OpenGLWindow::OpenGLWindow()
: m_qContext(nullptr)
, m_context(nullptr)
, m_initialized(false)
, m_updatePending(false)
{
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_context;
}

void OpenGLWindow::setContextFormat(const gloperate::ContextFormat & format)
{
    QSurfaceFormat qFormat;

    // Set OpenGL version
    glbinding::Version version = format.version();
    qFormat.setVersion(version.m_major, version.m_minor);

    // Set OpenGL profile
    switch (format.profile())
    {
        case gloperate::ContextFormat::Profile::Core:
            qFormat.setProfile(QSurfaceFormat::CoreProfile);
            break;

        case gloperate::ContextFormat::Profile::Compatibility:
            qFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
            break;

        default:
            qFormat.setProfile(QSurfaceFormat::NoProfile);
            break;
    }

    // Set buffer options
    qFormat.setDepthBufferSize(format.depthBufferSize());
    qFormat.setStencilBufferSize(format.stencilBufferSize());
    qFormat.setRedBufferSize(format.redBufferSize());
    qFormat.setGreenBufferSize(format.greenBufferSize());
    qFormat.setBlueBufferSize(format.blueBufferSize());
    qFormat.setAlphaBufferSize(format.alphaBufferSize());
    qFormat.setStereo(format.stereo());
    qFormat.setSamples(format.samples());

    // Create Qt context
    setContextFormat(qFormat);
}

void OpenGLWindow::setContextFormat(const QSurfaceFormat & format)
{
    m_qContext.reset(new QOpenGLContext);

    QSurfaceFormat f(format);
    f.setRenderableType(QSurfaceFormat::OpenGL);

    setSurfaceType(OpenGLSurface);
    create();

    if (f.version().first < 3)
    {
        m_qContext->setFormat(f);
        if (!m_qContext->create()) {
            qDebug() << "Could not create intermediate OpenGL context.";
            QApplication::quit();
        } else {
            QSurfaceFormat intermediateFormat = m_qContext->format();
            qDebug().nospace() << "Created intermediate OpenGL context " << intermediateFormat.version().first << "." << intermediateFormat.version().second;

            if ((intermediateFormat.version().first == 3 && intermediateFormat.version().second == 0) || intermediateFormat.version().first < 3)
            {
                f.setMajorVersion(3);
                f.setMinorVersion(2);
                f.setProfile(QSurfaceFormat::CoreProfile);
            }
        }
    }

    m_qContext->setFormat(f);
    if (!m_qContext->create()) {
        qDebug() << "Could not create OpenGL context.";
        QApplication::quit();
    } else {
        qDebug().nospace() << "Created OpenGL context " << m_qContext->format().version().first << "." << m_qContext->format().version().second;
    }
}

Context * OpenGLWindow::context() const
{
    if (!m_context)
    {
        m_context = new Context(const_cast<OpenGLWindow*>(this), &(*m_qContext));
    }

    return m_context;
}

void OpenGLWindow::updateGL()
{
    if (!m_updatePending) {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void OpenGLWindow::initialize()
{
    m_qContext->makeCurrent(this);

    onInitialize();

    m_qContext->doneCurrent();

    m_initialized = true;
}

void OpenGLWindow::resize(QResizeEvent * event)
{
    if (!m_initialized) {
        initialize();
    }

    m_qContext->makeCurrent(this);

    QResizeEvent deviceSpecificResizeEvent(event->size() * devicePixelRatio(), event->oldSize() * devicePixelRatio());

    onResize(&deviceSpecificResizeEvent);

    m_qContext->doneCurrent();
}

void OpenGLWindow::paint()
{
    if (!m_initialized) {
        initialize();
    }

    if (!isExposed()) {
        return;
    }

    m_updatePending = false;

    m_qContext->makeCurrent(this);

    onPaint();

    m_qContext->swapBuffers(this);

    m_qContext->doneCurrent();
}

void OpenGLWindow::onInitialize()
{
}

void OpenGLWindow::onResize(QResizeEvent * )
{
}

void OpenGLWindow::onPaint()
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
    paint();
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

void OpenGLWindow::makeCurrent()
{
    m_qContext->makeCurrent(this);
}

void OpenGLWindow::doneCurrent()
{
    m_qContext->doneCurrent();
}


} // namespace gloperate-qt
