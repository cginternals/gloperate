
#include <gloperate-glfw/RenderWindow.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gloperate/viewer/input.h>
#include <gloperate/viewer/RenderSurface.h>

#include <gloperate-glfw/WindowEvent.h>
#include <gloperate-glfw/GLContext.h>
#include <gloperate-glfw/input.h>


using namespace gloperate;


namespace gloperate_glfw
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
        destroy();
        m_surface->setRenderStage(stage);
        setContextFormat(m_surface->requiredFormat());
        create();
    }
}

RenderWindow::RenderWindow(
    gloperate::ViewerContext * viewerContext
  , gloperate::RenderSurface * surface)
: Window(surface->requiredFormat())
, m_viewerContext(viewerContext)
, m_surface(surface)
{
}

void RenderWindow::onContextInit()
{
    m_surface->setOpenGLContext(m_context);
}

void RenderWindow::onContextDeinit()
{
    m_surface->setOpenGLContext(nullptr);
}

void RenderWindow::onIdle()
{
    m_surface->onIdle();
}

void RenderWindow::onResize(ResizeEvent & event)
{
    m_virtualSize = event.size();

    m_surface->onResize(m_deviceSize, m_virtualSize);
}

void RenderWindow::onFramebufferResize(ResizeEvent & event)
{
    m_deviceSize = event.size();

    m_surface->onResize(m_deviceSize, m_virtualSize);
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
        fromGLFWKeyCode(event.key())
    );
}

void RenderWindow::onKeyRelease(KeyEvent & event)
{
    m_surface->onKeyRelease(
        fromGLFWKeyCode(event.key())
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

void RenderWindow::onTimer(TimerEvent &)
{
}


} // namespace gloperate_glfw
