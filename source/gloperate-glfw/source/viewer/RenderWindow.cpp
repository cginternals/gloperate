
#include <gloperate-glfw/viewer/RenderWindow.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gloperate/viewer/input.h>
#include <gloperate/viewer/RenderSurface.h>

#include <gloperate-glfw/viewer/WindowEvent.h>
#include <gloperate-glfw/viewer/GLContext.h>
#include <gloperate-glfw/viewer/input.h>


using namespace gloperate;


namespace gloperate_glfw
{


RenderWindow::RenderWindow(gloperate::ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
, m_surface(new RenderSurface(viewerContext))
{
    m_surface->redraw.connect([this] ()
    {
        repaint();
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

gloperate::RenderSurface * RenderWindow::renderSurface() const
{
    return m_surface;
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

void RenderWindow::onResize(ResizeEvent & event)
{
    m_virtualSize = event.size();

    m_surface->onViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
      , glm::vec4(0, 0, m_virtualSize.x, m_virtualSize.y)
    );
}

void RenderWindow::onFramebufferResize(ResizeEvent & event)
{
    m_deviceSize = event.size();

    m_surface->onViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
      , glm::vec4(0, 0, m_virtualSize.x, m_virtualSize.y)
    );
}

void RenderWindow::onMove(MoveEvent &)
{
}

void RenderWindow::onPaint(PaintEvent &)
{
    m_surface->onRender();
}

void RenderWindow::onKeyPress(KeyEvent & event)
{
    if (event.key() == GLFW_KEY_F11)
    {
      setFullscreen(!isFullscreen());
    }

    m_surface->onKeyPress(
        fromGLFWKeyCode(event.key()),
        fromGLFWModifier(event.modifiers())
    );
}

void RenderWindow::onKeyRelease(KeyEvent & event)
{
    m_surface->onKeyRelease(
        fromGLFWKeyCode(event.key()),
        fromGLFWModifier(event.modifiers())
    );
}

void RenderWindow::onMousePress(MouseEvent & event)
{
    m_surface->onMousePress(
        fromGLFWMouseButton(event.button())
      , event.pos()
    );
}

void RenderWindow::onMouseRelease(MouseEvent & event)
{
    m_surface->onMouseRelease(
        fromGLFWMouseButton(event.button())
      , event.pos()
    );
}

void RenderWindow::onMouseMove(MouseEvent & event)
{
    m_surface->onMouseMove(event.pos());
}

void RenderWindow::onMouseEnter(MouseEnterEvent &)
{
}

void RenderWindow::onMouseLeave(MouseLeaveEvent &)
{
}

void RenderWindow::onScroll(ScrollEvent & event)
{
    m_surface->onMouseWheel(
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
