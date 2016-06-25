
#include <gloperate-glfw/RenderWindow.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gloperate/base/Canvas.h>
#include <gloperate/input/constants.h>

#include <gloperate-glfw/GLContext.h>
#include <gloperate-glfw/WindowEvent.h>
#include <gloperate-glfw/input.h>


using namespace gloperate;


namespace gloperate_glfw
{


RenderWindow::RenderWindow(gloperate::Environment * environment)
: m_environment(environment)
, m_canvas(new Canvas(environment))
{
    m_canvas->redraw.connect([this] ()
    {
        repaint();
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

gloperate::Canvas * RenderWindow::canvas() const
{
    return m_canvas;
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

void RenderWindow::onResize(ResizeEvent & event)
{
    m_virtualSize = event.size();

    m_canvas->onViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
      , glm::vec4(0, 0, m_virtualSize.x, m_virtualSize.y)
    );
}

void RenderWindow::onFramebufferResize(ResizeEvent & event)
{
    m_deviceSize = event.size();

    m_canvas->onViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
      , glm::vec4(0, 0, m_virtualSize.x, m_virtualSize.y)
    );
}

void RenderWindow::onMove(MoveEvent &)
{
}

void RenderWindow::onPaint(PaintEvent &)
{
    m_canvas->onRender();
}

void RenderWindow::onKeyPress(KeyEvent & event)
{
    if (event.key() == GLFW_KEY_F11)
    {
      setFullscreen(!isFullscreen());
    }

    m_canvas->onKeyPress(
        fromGLFWKeyCode(event.key()),
        fromGLFWModifier(event.modifiers())
    );
}

void RenderWindow::onKeyRelease(KeyEvent & event)
{
    m_canvas->onKeyRelease(
        fromGLFWKeyCode(event.key()),
        fromGLFWModifier(event.modifiers())
    );
}

void RenderWindow::onMousePress(MouseEvent & event)
{
    m_canvas->onMousePress(
        fromGLFWMouseButton(event.button())
      , event.pos()
    );
}

void RenderWindow::onMouseRelease(MouseEvent & event)
{
    m_canvas->onMouseRelease(
        fromGLFWMouseButton(event.button())
      , event.pos()
    );
}

void RenderWindow::onMouseMove(MouseEvent & event)
{
    m_canvas->onMouseMove(event.pos());
}

void RenderWindow::onMouseEnter(MouseEnterEvent &)
{
}

void RenderWindow::onMouseLeave(MouseLeaveEvent &)
{
}

void RenderWindow::onScroll(ScrollEvent & event)
{
    m_canvas->onMouseWheel(
        event.offset()
      , event.pos()
    );
}

void RenderWindow::onFocus(FocusEvent &)
{
}

void RenderWindow::onIconify(IconifyEvent &)
{
}


} // namespace gloperate_glfw
