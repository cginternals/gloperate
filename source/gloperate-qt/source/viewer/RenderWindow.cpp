
#include "gloperate-qt/viewer/RenderWindow.h"

#include <glm/glm.hpp>

#include <QCoreApplication>

#include <gloperate/viewer/RenderSurface.h>
#include <gloperate/viewer/ViewerContext.h>

#include <gloperate-qt/viewer/GLContext.h>
#include <gloperate-qt/viewer/input.h>

#include <gloperate/input/InputEvent.h>


namespace gloperate_qt
{


RenderWindow::RenderWindow(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_surface(new gloperate::RenderSurface(viewerContext))
{
    m_surface->redraw.connect([this] ()
    {
        this->updateGL();
    } );

    m_surface->wakeup.connect([] ()
    {
        printf("wakeup\n");
        QCoreApplication::instance()->processEvents();
    } );
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
    m_surface->setRenderStage(stage);
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
        glm::vec4(0, 0, deviceSize.width(),  deviceSize.height())
      , glm::vec4(0, 0, virtualSize.width(), virtualSize.height())
    );
}

void RenderWindow::onPaint()
{
    m_surface->onRender();
}

void RenderWindow::keyPressEvent(QKeyEvent * event)
{
    m_surface->onKeyPress(
        fromQtKeyCode(event->key(), event->modifiers()),
        fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::keyReleaseEvent(QKeyEvent * event)
{
    m_surface->onKeyRelease(
        fromQtKeyCode(event->key(), event->modifiers()),
        fromQtModifiers(event->modifiers())
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
        glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                   event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}


} // namespace gloperate-qt
