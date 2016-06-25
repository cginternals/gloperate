
#include "gloperate-qt/base/RenderWindow.h"

#include <glm/glm.hpp>

#include <QCoreApplication>

#include <gloperate/base/Environment.h>
#include <gloperate/base/Canvas.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/input.h>

#include <gloperate/input/InputEvent.h>


namespace gloperate_qt
{


RenderWindow::RenderWindow(gloperate::Environment * environment)
: m_environment(environment)
, m_canvas(new gloperate::Canvas(environment))
{
    m_canvas->redraw.connect([this] ()
    {
        this->updateGL();
    } );
}

RenderWindow::~RenderWindow()
{
    delete m_canvas;
}

gloperate::Environment * RenderWindow::environment() const
{
    return m_environment;
}

gloperate::Stage * RenderWindow::renderStage() const
{
    return m_canvas->renderStage();
}

void RenderWindow::setRenderStage(gloperate::Stage * stage)
{
    m_canvas->setRenderStage(stage);
}

void RenderWindow::onContextInit()
{
    m_canvas->setOpenGLContext(m_context);
}

void RenderWindow::onContextDeinit()
{
    m_canvas->setOpenGLContext(nullptr);
}

void RenderWindow::onResize(const QSize & deviceSize, const QSize & virtualSize)
{
    m_canvas->onViewport(
        glm::vec4(0, 0, deviceSize.width(),  deviceSize.height())
      , glm::vec4(0, 0, virtualSize.width(), virtualSize.height())
    );
}

void RenderWindow::onPaint()
{
    m_canvas->onRender();
}

void RenderWindow::keyPressEvent(QKeyEvent * event)
{
    m_canvas->onKeyPress(
        fromQtKeyCode(event->key(), event->modifiers()),
        fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::keyReleaseEvent(QKeyEvent * event)
{
    m_canvas->onKeyRelease(
        fromQtKeyCode(event->key(), event->modifiers()),
        fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::mouseMoveEvent(QMouseEvent * event)
{
    m_canvas->onMouseMove(glm::ivec2(
        (int)(event->x() * devicePixelRatio()),
        (int)(event->y() * devicePixelRatio()))
    );
}

void RenderWindow::mousePressEvent(QMouseEvent * event)
{
    m_canvas->onMousePress(
        fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}

void RenderWindow::mouseReleaseEvent(QMouseEvent * event)
{
    m_canvas->onMouseRelease(
        fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}

void RenderWindow::wheelEvent(QWheelEvent * event)
{
    m_canvas->onMouseWheel(
        glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                   event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}


} // namespace gloperate-qt
