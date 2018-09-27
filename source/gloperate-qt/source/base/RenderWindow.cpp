
#include "gloperate-qt/base/RenderWindow.h"

#include <glm/glm.hpp>

#include <globjects/Framebuffer.h>

#include <QOpenGLContext>

#include <gloperate/base/Canvas.h>
#include <gloperate/base/Environment.h>

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

gloperate::Canvas * RenderWindow::canvas() const
{
    return m_canvas.get();
}

void RenderWindow::onContextInit()
{
    m_canvas->setOpenGLContext(m_context.get());
    m_framebuffer = globjects::Framebuffer::defaultFBO();
}

void RenderWindow::onContextDeinit()
{
    m_canvas->setOpenGLContext(nullptr);
    m_framebuffer = nullptr;
}

void RenderWindow::onResize(const QSize & deviceSize, const QSize & /*virtualSize*/)
{
    m_canvas->setViewport(
        glm::vec4(0, 0, deviceSize.width(),  deviceSize.height())
    );
}

void RenderWindow::onPaint()
{
    m_canvas->render(m_framebuffer.get());
}

void RenderWindow::onTimer()
{
    m_canvas->updateTime();
}

void RenderWindow::keyPressEvent(QKeyEvent * event)
{
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    m_canvas->promoteKeyPress(
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

    m_canvas->promoteKeyRelease(
        Converter::fromQtKeyCode(event->key(), event->modifiers()),
        Converter::fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::mouseMoveEvent(QMouseEvent * event)
{
    m_canvas->promoteMouseMove(glm::ivec2(
        (int)(event->x() * devicePixelRatio()),
        (int)(event->y() * devicePixelRatio())),
        Converter::fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::mousePressEvent(QMouseEvent * event)
{
    m_canvas->promoteMousePress(
        Converter::fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) ),
        Converter::fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::mouseReleaseEvent(QMouseEvent * event)
{
    m_canvas->promoteMouseRelease(
        Converter::fromQtMouseButton(event->button()),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) ),
        Converter::fromQtModifiers(event->modifiers())
    );
}

void RenderWindow::wheelEvent(QWheelEvent * event)
{
    m_canvas->promoteMouseWheel(
        glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                   event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
        glm::ivec2( (int)(event->x() * devicePixelRatio()),
                    (int)(event->y() * devicePixelRatio()) ),
        Converter::fromQtModifiers(event->modifiers())
    );
}


} // namespace gloperate_qt
