
#include "gloperate-qt/base/RenderWindow.h"

#include <glm/glm.hpp>

#include <globjects/Framebuffer.h>

#include <QOpenGLContext>

#include <gloperate/base/Canvas.h>

#include <gloperate-qt/base/GLContext.h>
#include <gloperate-qt/base/Converter.h>


namespace gloperate_qt
{


RenderWindow::RenderWindow(gloperate::Environment * environment)
: m_environment(environment)
, m_canvas(cppassist::make_unique<gloperate::Canvas>(environment))
{
    m_canvas->redraw.connect([this] ()
    {
        this->updateGL();
    } );
}

RenderWindow::~RenderWindow()
{
}

gloperate::Environment * RenderWindow::environment() const
{
    return m_environment;
}

gloperate::Stage * RenderWindow::renderStage() const
{
    return m_canvas->renderStage();
}

void RenderWindow::setRenderStage(std::unique_ptr<gloperate::Stage> && stage)
{
    m_context->qtContext()->makeCurrent(this);
    m_canvas->setRenderStage(std::move(stage));
    m_context->qtContext()->doneCurrent();
}

void RenderWindow::onContextInit()
{
    m_canvas->setOpenGLContext(m_context.get());
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
    // [TODO]: optimize memory reallocation problem
    auto defaultFBO = globjects::Framebuffer::defaultFBO();

    m_canvas->onRender(defaultFBO.get());
}

void RenderWindow::keyPressEvent(QKeyEvent * event)
{
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    m_canvas->onKeyPress(
        Converter::fromQtKeyCode(event->key(), event->modifiers()),
        Converter::fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::keyReleaseEvent(QKeyEvent * event)
{
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    m_canvas->onKeyRelease(
        Converter::fromQtKeyCode(event->key(), event->modifiers()),
        Converter::fromQtModifiers(event->modifiers())
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
        Converter::fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) )
    );
}

void RenderWindow::mouseReleaseEvent(QMouseEvent * event)
{
    m_canvas->onMouseRelease(
        Converter::fromQtMouseButton(event->button()),
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
