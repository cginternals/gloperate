
#include <gloperate-qtquick/QuickView.h>

#include <QSurfaceFormat>
#include <QOpenGLContext>

#include <globjects/base/baselogging.h>

#include <gloperate/base/GLContextUtils.h>
#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/pipeline/Stage.h>

#include <gloperate-qtquick/RenderItem.h>
#include <gloperate-qtquick/Utils.h>

    
namespace gloperate_qtquick
{


QuickView::QuickView(
    gloperate::ViewerContext * viewerContext
  , gloperate::Stage * renderStage)
: QuickView(viewerContext, new gloperate::RenderSurface(viewerContext, renderStage))
{
    qmlRegisterType<RenderItem>("GLOperate", 1, 0, "RenderItem");

    connect(
        this, &QQuickView::sceneGraphInitialized,
        this, &QuickView::onSceneGraphInitialized,
        Qt::DirectConnection
    );

    connect(
        this, &QQuickView::openglContextCreated,
        this, &QuickView::onOpenglContextCreated,
        Qt::DirectConnection
    );
}

QuickView::~QuickView()
{
    delete m_surface;
}

gloperate::ViewerContext * QuickView::viewerContext() const
{
    return m_viewerContext;
}

gloperate::Stage * QuickView::renderStage() const
{
    return m_surface->renderStage();
}

void QuickView::setRenderStage(gloperate::Stage * stage)
{
    m_surface->setRenderStage(stage);

    /*
    // De-initialize and destroy old render stage
    m_surface->setRenderStage(nullptr);

    // Create new context for render stage and initialize render stage
    if (stage)
    {
        destroyContext();
        m_surface->setRenderStage(stage);
        setContextFormat(m_surface->requiredFormat());
        createContext();
    }

    // Start update timer
    m_timer.start(0);
    */
}

QuickView::QuickView(
    gloperate::ViewerContext * viewerContext
  , gloperate::RenderSurface * surface)
: m_viewerContext(viewerContext)
, m_surface(surface)
{
    /*
    m_surface->redrawNeeded.connect([this] ()
    {
        this->updateGL();
    } );

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    m_timer.setSingleShot(true);
    m_timer.start(0);
    */
}

void QuickView::onSceneGraphInitialized()
{
    QSurfaceFormat format = requestedFormat();
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(4, 4);
    format.setProfile(QSurfaceFormat::CoreProfile);

    openglContext()->doneCurrent();
    openglContext()->setFormat(format);
    openglContext()->create();
    openglContext()->makeCurrent(this);

    openglContext()->makeCurrent(this);

    Utils::initContext();

    globjects::info() << std::endl
        << "OpenGL Version:  " << gloperate::GLContextUtils::version() << std::endl
        << "OpenGL Vendor:   " << gloperate::GLContextUtils::vendor() << std::endl
        << "OpenGL Renderer: " << gloperate::GLContextUtils::renderer() << std::endl;

    if (m_surface && m_surface->renderStage())
    {
        m_surface->renderStage()->initContext(nullptr);
    }

    openglContext()->doneCurrent();
}

void QuickView::onOpenglContextCreated(QOpenGLContext *)
{
}


} // namespace gloperate_qtquick
