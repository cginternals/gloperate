
#include <gloperate-glfw/RenderWindow.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <gloperate/viewer/input.h>
#include <gloperate/viewer/DemoRenderer.h>

#include <gloperate-glfw/WindowEvent.h>
#include <gloperate-glfw/GLContext.h>


using namespace gloperate;


namespace gloperate_glfw
{


/**
*  @brief
*    Convert GLFW mouse button into gloperate mouse button
*/
static gloperate::MouseButton fromGLFWMouseButton(int button)
{
    switch (button) 
    {
        case GLFW_MOUSE_BUTTON_1: return MouseButton1;
        case GLFW_MOUSE_BUTTON_2: return MouseButton2;
        case GLFW_MOUSE_BUTTON_3: return MouseButton3;
        case GLFW_MOUSE_BUTTON_4: return MouseButton4;
        case GLFW_MOUSE_BUTTON_5: return MouseButton5;
        case GLFW_MOUSE_BUTTON_6: return MouseButton6;
        case GLFW_MOUSE_BUTTON_7: return MouseButton7;
        case GLFW_MOUSE_BUTTON_8: return MouseButton8;
        default:                  return NoMouseButton;
    }
}

/**
*  @brief
*    Convert GLFW key code into gloperate key code
*/
static gloperate::Key fromGLFWKeyCode(int key)
{
    // We are using the same key code table as GLFW
    return static_cast<gloperate::Key>(key);
}


RenderWindow::RenderWindow(gloperate::ViewerContext * viewerContext)
: m_surface(new gloperate::DemoRenderer(viewerContext))
{
}

RenderWindow::~RenderWindow()
{
    delete m_surface;
}

gloperate::Surface * RenderWindow::surface() const
{
    return m_surface;
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
