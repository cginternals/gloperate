
#include "gloperate-qt/viewer/OpenGLWindow.h"

#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QOpenGLContext>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-qt/viewer/Context.h>


namespace gloperate_qt
{


OpenGLWindow::OpenGLWindow()
: m_qContext(nullptr)
, m_context(nullptr)
, m_initialized(false)
, m_updatePending(false)
{
    setSurfaceType(OpenGLSurface);
    create();
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_context;
}

void OpenGLWindow::setContextFormat(const gloperate::GLContextFormat & format)
{
    QSurfaceFormat qFormat;
    qFormat.setVersion(3, 2);
    qFormat.setProfile(QSurfaceFormat::CoreProfile);
    qFormat.setDepthBufferSize(16);

    // Set OpenGL version
    glbinding::Version version = format.version();
    qFormat.setVersion(version.m_major, version.m_minor);

    // Set OpenGL profile
    switch (format.profile())
    {
        case gloperate::GLContextFormat::Profile::Core:
            qFormat.setProfile(QSurfaceFormat::CoreProfile);
            break;

        case gloperate::GLContextFormat::Profile::Compatibility:
            qFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
            break;

        default:
            qFormat.setProfile(QSurfaceFormat::NoProfile);
            break;
    }

    // Set buffer options
    qFormat.setRedBufferSize(format.redBufferSize());
    qFormat.setGreenBufferSize(format.greenBufferSize());
    qFormat.setBlueBufferSize(format.blueBufferSize());

    // [TODO] [BUG] When alphaBufferSize is set to 8, the context is not created.
//  qFormat.setAlphaBufferSize(format.alphaBufferSize());
    qFormat.setAlphaBufferSize(16);

    qFormat.setDepthBufferSize(format.depthBufferSize());
    qFormat.setStencilBufferSize(format.stencilBufferSize());
    qFormat.setStereo(format.stereo());
    qFormat.setSamples(format.samples());

    // Create Qt context
    setContextFormat(qFormat);
}

void OpenGLWindow::setContextFormat(const QSurfaceFormat & format)
{
    m_format = format;
}

void OpenGLWindow::doIt()
{
    m_qContext.reset(new QOpenGLContext);

    QSurfaceFormat format(m_format);
    format.setRenderableType(QSurfaceFormat::OpenGL);

    if (format.version().first < 3)
    {
        m_qContext->setFormat(format);
        if (!m_qContext->create()) {
            qDebug() << "Could not create intermediate OpenGL context.";
            QApplication::quit();
        } else {
            QSurfaceFormat intermediateFormat = m_qContext->format();
            qDebug() << "Created intermediate OpenGL context " << intermediateFormat.version().first << "." << intermediateFormat.version().second;

            if ((intermediateFormat.version().first == 3 && intermediateFormat.version().second == 0) || intermediateFormat.version().first < 3)
            {
                format.setMajorVersion(3);
                format.setMinorVersion(2);
                format.setProfile(QSurfaceFormat::CoreProfile);
            }
        }
    }

    m_qContext->setFormat(format);
    if (!m_qContext->create()) {
        qDebug() << "Could not create OpenGL context.";
        QApplication::quit();
    } else {
        qDebug() << "Created OpenGL context " << m_qContext->format().version().first << "." << m_qContext->format().version().second;
    }

    m_context = new Context(this, &(*m_qContext));
}

Context * OpenGLWindow::context() const
{
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
