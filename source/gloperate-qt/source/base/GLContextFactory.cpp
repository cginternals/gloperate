
#include <gloperate-qt/base/GLContextFactory.h>

#include <QWindow>
#include <QOpenGLContext>

#include <cppassist/logging/logging.h>

#include <gloperate-qt/base/GLContext.h>


namespace gloperate_qt
{


QSurfaceFormat GLContextFactory::toQSurfaceFormat(const gloperate::GLContextFormat & format)
{
    QSurfaceFormat qFormat;

    // Set OpenGL version
    glbinding::Version version = format.version();
    qFormat.setVersion(version.majorVersion(), version.minorVersion());

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
    qFormat.setAlphaBufferSize(format.alphaBufferSize());
    qFormat.setDepthBufferSize(format.depthBufferSize());
    qFormat.setStencilBufferSize(format.stencilBufferSize());
    qFormat.setStereo(format.stereo());
    qFormat.setSamples(format.samples());

    // Return format description
    return qFormat;
}

GLContextFactory::GLContextFactory(QWindow * window)
: m_window(window)
{
}

GLContextFactory::~GLContextFactory()
{
}

gloperate::AbstractGLContext * GLContextFactory::createContext(const gloperate::GLContextFormat & format)
{
    // Get Qt context format
    QSurfaceFormat qFormat = toQSurfaceFormat(format);
    qFormat.setRenderableType(QSurfaceFormat::OpenGL);

    // Create OpenGL context
    QOpenGLContext * qContext = new QOpenGLContext;

    if (qFormat.version().first < 3)
    {
        qContext->setFormat(qFormat);

        if (!qContext->create()) {
            cppassist::warning() << "Could not create intermediate OpenGL context.";
            return nullptr;
        } else {
            QSurfaceFormat intermediateFormat = qContext->format();
            cppassist::info() << "Created intermediate OpenGL context " << intermediateFormat.version().first << "." << intermediateFormat.version().second;

            if ((intermediateFormat.version().first == 3 && intermediateFormat.version().second == 0) || intermediateFormat.version().first < 3)
            {
                qFormat.setMajorVersion(3);
                qFormat.setMinorVersion(2);
                qFormat.setProfile(QSurfaceFormat::CoreProfile);
            }
        }
    }

    qContext->setFormat(qFormat);
    if (!qContext->create()) {
        cppassist::warning() << "Could not create OpenGL context.";
        return nullptr;
    } else {
        cppassist::info() << "Created OpenGL context " << qContext->format().version().first << "." << qContext->format().version().second;
    }

    // Activate context
    GLContext::makeCurrent(qContext, m_window);

    // Initialize glbinding in that context
    GLContext::initGLBinding();

    // Create context wrapper
    GLContext * context = new GLContext(m_window, qContext);

    // Deactivate context
    GLContext::doneCurrent(qContext);

    return context;
}


} // namespace gloperate_qt
