
#include <gloperate-qt/base/GLContextFactory.h>

#include <QOpenGLContext>
#include <QSurfaceFormat>

#include <cppassist/memory/make_unique.h>

#include <gloperate-qt/base/GLContext.h>


namespace gloperate_qt
{


GLContextFactory::GLContextFactory(QSurface * surface)
: m_surface(surface)
{
}

GLContextFactory::~GLContextFactory()
{
}

std::unique_ptr<gloperate::AbstractGLContext> GLContextFactory::createContext(const gloperate::GLContextFormat & format) const
{
    // Create OpenGL context
    auto qContext = cppassist::make_unique<QOpenGLContext>();
    qContext->setFormat(toQSurfaceFormat(format));

    // Create and check context
    if (!qContext->create())
    {
        return nullptr;
    }

    return cppassist::make_unique<GLContext>(m_surface, qContext.release());
}

QSurfaceFormat GLContextFactory::toQSurfaceFormat(const gloperate::GLContextFormat & format)
{

    QSurfaceFormat qFormat;
    qFormat.setRenderableType(QSurfaceFormat::OpenGL);

    // Set OpenGL version
    qFormat.setVersion(format.majorVersion(), format.minorVersion());

    // Set OpenGL context flags
    if (format.version() >= glbinding::Version(3, 0))
    {
        qFormat.setOption(QSurfaceFormat::DeprecatedFunctions, !format.forwardCompatible());
        qFormat.setOption(QSurfaceFormat::DebugContext, format.debugContext());
    }

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

    // Handle swap behavior
    switch (format.swapBehavior())
    {
    case gloperate::GLContextFormat::SwapBehavior::DoubleBuffering:
        qFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        break;
    case gloperate::GLContextFormat::SwapBehavior::SingleBuffering:
        qFormat.setSwapBehavior(QSurfaceFormat::SingleBuffer);
        break;
    default:
        qFormat.setSwapBehavior(QSurfaceFormat::DefaultSwapBehavior);
    }

    // Return format description
    return qFormat;
}


} // namespace gloperate_qt
