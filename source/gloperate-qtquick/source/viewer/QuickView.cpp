
#include <gloperate-qtquick/viewer/QuickView.h>

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QQmlEngine>
#include <QVariant>
#include <QColor>

#include <globjects/base/baselogging.h>

#include <gloperate/gloperate.h>
#include <gloperate/viewer/GLContextUtils.h>
#include <gloperate/viewer/GLContextFormat.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/GLContextFactory.h>

#include <gloperate-qtquick/controls/TextController.h>
#include <gloperate-qtquick/viewer/RenderItem.h>
#include <gloperate-qtquick/viewer/Utils.h>

    
namespace gloperate_qtquick
{


QuickView::QuickView(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_context(nullptr)
{
    // Do not clear surface when rendering Qml, because we render our content first
    setClearBeforeRendering(false);

    // Register QML types
    qmlRegisterType<RenderItem>    ("gloperate.rendering", 1, 0, "RenderItem");
    qmlRegisterType<TextController>("gloperate.ui",        1, 0, "TextController");

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
    // Get background color
    QVariant var = rootObject()->property("backgroundColor");
    QColor color = var.value<QColor>();

    // Clear screen
    Utils::clearScreen(color.redF(), color.greenF(), color.blueF());
}


} // namespace gloperate_qtquick
