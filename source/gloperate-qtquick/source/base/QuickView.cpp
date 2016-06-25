
#include <gloperate-qtquick/base/QuickView.h>

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QVariant>
#include <QColor>
#include <QQuickItem>

#include <cppassist/logging/logging.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/GLContextFactory.h>

#include <gloperate-qtquick/scripting/QmlEngine.h>
#include <gloperate-qtquick/base/Utils.h>


using namespace cppassist;


namespace gloperate_qtquick
{


QuickView::QuickView(QmlEngine * engine, QWindow * parent)
: QQuickView(engine, parent)
, m_environment(engine->environment())
, m_context(nullptr)
{
    // Do not clear surface when rendering Qml, because we render our content first
    setClearBeforeRendering(false);

    // Connect to context creation and scene graph initialization
    connect(
        this, &QQuickView::sceneGraphInitialized,
        this, &QuickView::onSceneGraphInitialized,
        Qt::DirectConnection
    );

    // Connect to window draw event
    connect(
        this, &QQuickWindow::beforeRendering,
        this, &QuickView::onBeforeRendering,
        Qt::DirectConnection
    );

    // Determine OpenGL context format
    gloperate::GLContextFormat format;
    QSurfaceFormat qFormat = gloperate_qt::GLContextFactory::toQSurfaceFormat(
        format
    );
    setFormat(qFormat);
}

QuickView::~QuickView()
{
    delete m_context;
}

gloperate::Environment * QuickView::environment() const
{
    return m_environment;
}

gloperate_qt::GLContext * QuickView::context() const
{
    return m_context;
}

void QuickView::onSceneGraphInitialized()
{
    // Activate context
    openglContext()->makeCurrent(this);

    // Initialize glbinding and globjects in context
    Utils::initContext();

    // Print context info
    info() << std::endl
        << "OpenGL Version:  " << gloperate::GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << gloperate::GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << gloperate::GLContextUtils::renderer() << std::endl;

    // Create context wrapper
    m_context = new gloperate_qt::GLContext(this, openglContext(), false);
}

void QuickView::onBeforeRendering()
{
    // Check if view is still valid (this may fail on shutdown)
    if (!rootObject())
    {
        return;
    }

    // Get background color
    QVariant var = rootObject()->property("backgroundColor");
    QColor color = var.value<QColor>();

    // Clear screen
    Utils::clearScreen(color.redF(), color.greenF(), color.blueF());
}


} // namespace gloperate_qtquick
