
#include <gloperate-qtquick/QuickView.h>

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QQmlEngine>

#include <globjects/base/baselogging.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/GLContextUtils.h>
#include <gloperate/base/GLContextFormat.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/GLContextFactory.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate-qtquick/Utils.h>

    
namespace gloperate_qtquick
{


QuickView::QuickView(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_context(nullptr)
{
    // Register QML types
    qmlRegisterType<RenderItem>("GLOperate.Rendering", 1, 0, "RenderItem");

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

    // Add gloperate qml-libraries
    std::string importPath = gloperate::dataPath() + "/gloperate/qml/GLOperate/Ui";
    engine()->addImportPath(QString::fromStdString(importPath));
}

QuickView::~QuickView()
{
    delete m_context;
}

gloperate::ViewerContext * QuickView::viewerContext() const
{
    return m_viewerContext;
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
    globjects::info() << std::endl
        << "OpenGL Version:  " << gloperate::GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << gloperate::GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << gloperate::GLContextUtils::renderer() << std::endl;

    // Create context wrapper
    m_context = new gloperate_qt::GLContext(this, openglContext(), false);
}

void QuickView::onBeforeRendering()
{
    Utils::clearScreen();
}


} // namespace gloperate_qtquick
