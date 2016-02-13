
#include <gloperate-qt/viewer/GLContextFactory.h>

#include <QOpenGLContext>

#include <globjects/base/baselogging.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/OpenGLWindow.h>


namespace gloperate_qt
{


GLContextFactory::GLContextFactory(OpenGLWindow * window)
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
            globjects::warning() << "Could not create intermediate OpenGL context.";
            return nullptr;
        } else {
            QSurfaceFormat intermediateFormat = qContext->format();
            globjects::info() << "Created intermediate OpenGL context " << intermediateFormat.version().first << "." << intermediateFormat.version().second;

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
        globjects::warning() << "Could not create OpenGL context.";
        return nullptr;
    } else {
        globjects::info() << "Created OpenGL context " << qContext->format().version().first << "." << qContext->format().version().second;
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

QSurfaceFormat GLContextFactory::toQSurfaceFormat(const gloperate::GLContextFormat & format) const
{
    QSurfaceFormat qFormat;

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

    // Return format description
    return qFormat;
}


} // namespace gloperate_qt
