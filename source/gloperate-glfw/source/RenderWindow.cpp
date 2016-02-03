
#include <gloperate-glfw/RenderWindow.h>

#include <gloperate/viewer/DemoRenderer.h>

#include <gloperate-glfw/WindowEvent.h>


using namespace gloperate;


namespace gloperate_glfw
{


RenderWindow::RenderWindow()
: m_renderHandler(new gloperate::DemoRenderer)
{
}

RenderWindow::~RenderWindow()
{
    delete m_renderHandler;
}

void RenderWindow::onInitialize()
{
    m_renderHandler->onContextInit();
}

void RenderWindow::onFinalize()
{
    m_renderHandler->onContextDeinit();
}

void RenderWindow::onIdle()
{
}

void RenderWindow::onResize(ResizeEvent &)
{
}

void RenderWindow::onFramebufferResize(ResizeEvent & event)
{
    m_renderHandler->onResize(event.width(), event.height());
}

void RenderWindow::onMove(MoveEvent &)
{
}

void RenderWindow::onPaint(PaintEvent &)
{
    m_renderHandler->onRender();
}

void RenderWindow::onKeyPress(KeyEvent & event)
{
    m_renderHandler->onKeyPressed(event.key());
}

void RenderWindow::onKeyRelease(KeyEvent & event)
{
    m_renderHandler->onKeyReleased(event.key());
}

void RenderWindow::onMousePress(MouseEvent & event)
{
    m_renderHandler->onMousePressed(event.button(), event.x(), event.x());
}

void RenderWindow::onMouseRelease(MouseEvent & event)
{
    m_renderHandler->onMouseReleased(event.button(), event.x(), event.x());
}

void RenderWindow::onMouseMove(MouseEvent & event)
{
    m_renderHandler->onMouseMoved(event.x(), event.x());
}

void RenderWindow::onMouseEnter(MouseEnterEvent &)
{
}

void RenderWindow::onMouseLeave(MouseLeaveEvent &)
{
}

void RenderWindow::onScroll(ScrollEvent &)
{
}

void RenderWindow::onFocus(FocusEvent &)
{
}

void RenderWindow::onIconify(IconifyEvent &)
{
}

void RenderWindow::onTimer(TimerEvent &)
{
}


} // namespace gloperate_glfw
