
#include "gloperate-qt/viewer/RenderWindow.h"

#include <glm/glm.hpp>

#include <gloperate/viewer/RenderSurface.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/input.h>


namespace gloperate_qt
{


RenderWindow::RenderWindow(
    gloperate::ViewerContext * viewerContext
  , gloperate::Stage * renderStage)
: RenderWindow(viewerContext, new gloperate::RenderSurface(viewerContext, renderStage))
{
}

RenderWindow::~RenderWindow()
{
    delete m_surface;
}

gloperate::ViewerContext * RenderWindow::viewerContext() const
{
    return m_viewerContext;
}

gloperate::Stage * RenderWindow::renderStage() const
{
    return m_surface->renderStage();
}

void RenderWindow::setRenderStage(gloperate::Stage * stage)
{
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
}

RenderWindow::RenderWindow(
    gloperate::ViewerContext * viewerContext
  , gloperate::RenderSurface * surface)
: OpenGLWindow(surface->requiredFormat())
, m_viewerContext(viewerContext)
, m_surface(surface)
{
    m_surface->redrawNeeded.connect([this] ()
    {
        this->updateGL();
    } );

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    m_timer.setSingleShot(true);
    m_timer.start(0);
}

void RenderWindow::onUpdate()
{
    if (m_surface->onUpdate())
    {
        m_timer.start(0);
    }
}

void RenderWindow::onContextInit()
{
    m_surface->setOpenGLContext(m_context);
}

void RenderWindow::onContextDeinit()
{
    m_surface->setOpenGLContext(nullptr);
}

void RenderWindow::onResize(const QSize & deviceSize, const QSize & virtualSize)
{
    m_surface->onViewport(
        glm::ivec4(0, 0, deviceSize.width(),  deviceSize.height())
      , glm::ivec4(0, 0, virtualSize.width(), virtualSize.height())
    );
}

void RenderWindow::onPaint()
{
    m_surface->onRender();
}

void RenderWindow::keyPressEvent(QKeyEvent * event)
{
    m_surface->onKeyPress(
        fromQtKeyCode(event->key(), event->modifiers())
    );
}

void RenderWindow::keyReleaseEvent(QKeyEvent * event)
{
    m_surface->onKeyRelease(
        fromQtKeyCode(event->key(), event->modifiers())
    );
}

void RenderWindow::mouseMoveEvent(QMouseEvent * event)
{
    m_surface->onMouseMove(glm::ivec2(
        (int)(event->x() * devicePixelRatio()),
        (int)(event->y() * devicePixelRatio()))
    );
}

void RenderWindow::mousePressEvent(QMouseEvent * event)
{
    m_surface->onMousePress(
        fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}

void RenderWindow::mouseReleaseEvent(QMouseEvent * event)
{
    m_surface->onMouseRelease(
        fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}

void RenderWindow::wheelEvent(QWheelEvent * event)
{
    m_surface->onMouseWheel(
        glm::ivec2( event->orientation() == Qt::Vertical ? 0 : event->delta(),
                    event->orientation() == Qt::Vertical ? event->delta() : 0 ),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}


} // namespace gloperate-qt
