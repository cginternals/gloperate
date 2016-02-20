
#include <gloperate-qtquick/QuickView.h>

#include <QSurfaceFormat>
#include <QOpenGLContext>

#include <globjects/base/baselogging.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/viewer/RenderSurface.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/GLContextFactory.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate-qtquick/Utils.h>

    
namespace gloperate_qtquick
{


QuickView::QuickView(
    gloperate::ViewerContext * viewerContext
  , gloperate::Stage * renderStage)
: QuickView(viewerContext, new gloperate::RenderSurface(viewerContext, renderStage))
{
}

QuickView::~QuickView()
{
    delete m_surface;
    delete m_context;
}

gloperate::ViewerContext * QuickView::viewerContext() const
{
    return m_viewerContext;
}

gloperate::Surface * QuickView::surface() const
{
    return m_surface;
}

gloperate::Stage * QuickView::renderStage() const
{
    return m_surface->renderStage();
}

void QuickView::setRenderStage(gloperate::Stage * stage)
{
    // De-initialize and destroy old render stage
    m_surface->setRenderStage(nullptr);

    // Initialize new render stage
    if (stage)
    {
        m_surface->setRenderStage(stage);
    }

    // Start update timer
    m_timer.start(0);
}

gloperate_qt::GLContext * QuickView::context() const
{
    return m_context;
}

QuickView::QuickView(
    gloperate::ViewerContext * viewerContext
  , gloperate::RenderSurface * surface)
: m_viewerContext(viewerContext)
, m_surface(surface)
, m_context(nullptr)
{
    // Register QML types
    qmlRegisterType<RenderItem>("GLOperate", 1, 0, "RenderItem");

    // Connect to context creation and scene graph initialization
    connect(
        this, &QQuickView::sceneGraphInitialized,
        this, &QuickView::onSceneGraphInitialized,
        Qt::DirectConnection
    );

    // Determine OpenGL context format
    QSurfaceFormat format = gloperate_qt::GLContextFactory::toQSurfaceFormat(
        m_surface->requiredFormat()
    );
    // [TODO] Bug: Somehow, only core contexts work
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);

    // Repaint window when surface needs to be updated
    m_surface->redrawNeeded.connect([this] ()
    {
        this->update();
    } );

    // Connect update timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    m_timer.setSingleShot(true);
    m_timer.start(0);
}

void QuickView::onUpdate()
{
    if (m_surface->onUpdate())
    {
        m_timer.start(0);
    }
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

    // Make current again
    openglContext()->makeCurrent(this);

    // Initialize render stage in context
    m_surface->setOpenGLContext(m_context);

    // De-activate context
    openglContext()->doneCurrent();
}


} // namespace gloperate_qtquick
