
#include <gloperate-glfw/RenderWindow.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <globjects/Framebuffer.h>

#include <gloperate/base/Canvas.h>
#include <gloperate/base/Environment.h>

#include <gloperate-glfw/GLContext.h>
#include <gloperate-glfw/WindowEvent.h>


using namespace gloperate;


namespace gloperate_glfw
{


RenderWindow::RenderWindow(gloperate::Environment * environment)
: m_environment(environment)
, m_canvas(cppassist::make_unique<Canvas>(environment))
{
    m_canvas->redraw.connect([this] ()
    {
        repaint();
    } );
}

RenderWindow::~RenderWindow()
{
}

const gloperate::Environment * RenderWindow::environment() const
{
    return m_environment;
}

gloperate::Environment * RenderWindow::environment()
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
}

void RenderWindow::onContextDeinit()
{
    m_canvas->setOpenGLContext(nullptr);
}

void RenderWindow::onResize(ResizeEvent & event)
{
    m_virtualSize = event.size();
}

void RenderWindow::onFramebufferResize(ResizeEvent & event)
{
    m_deviceSize = event.size();

    m_canvas->setViewport(
        glm::vec4(0, 0, m_deviceSize.x,  m_deviceSize.y)
    );
}

void RenderWindow::onMove(MoveEvent &)
{
}

void RenderWindow::onPaint(PaintEvent &)
{
    // [TODO] Optimize memory reallocation problem
    auto defaultFBO = globjects::Framebuffer::defaultFBO();

    m_canvas->render(defaultFBO.get());
}

void RenderWindow::onKeyPress(KeyEvent & event)
{
    // Skip auto-repeated key events
    if (event.action() == GLFW_REPEAT)
    {
        return;
    }

    m_canvas->promoteKeyPress(
        fromGLFWKeyCode(event.key()),
        fromGLFWModifier(event.modifiers())
    );

    if (event.key() == GLFW_KEY_F11 || (event.key() == GLFW_KEY_ENTER && (event.modifiers() & GLFW_MOD_ALT) != 0) )
    {
        setFullscreen(!isFullscreen());
    }

    if (event.key() == GLFW_KEY_ESCAPE)
    {
        close();
    }
}

void RenderWindow::onKeyRelease(KeyEvent & event)
{
    m_canvas->promoteKeyRelease(
        fromGLFWKeyCode(event.key()),
        fromGLFWModifier(event.modifiers())
    );
}

void RenderWindow::onMousePress(MouseEvent & event)
{
    m_canvas->promoteMousePress(
        fromGLFWMouseButton(event.button())
      , event.pos()
    );
}

void RenderWindow::onMouseRelease(MouseEvent & event)
{
    m_canvas->promoteMouseRelease(
        fromGLFWMouseButton(event.button())
      , event.pos()
    );
}

void RenderWindow::onMouseMove(MouseEvent & event)
{
    m_canvas->promoteMouseMove(event.pos());
}

void RenderWindow::onMouseEnter(MouseEnterEvent &)
{
}

void RenderWindow::onMouseLeave(MouseLeaveEvent &)
{
}

void RenderWindow::onScroll(ScrollEvent & event)
{
    m_canvas->promoteMouseWheel(
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

void RenderWindow::onIdle()
{
    m_canvas->updateTime();
}

gloperate::MouseButton RenderWindow::fromGLFWMouseButton(int button) const
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

gloperate::Key RenderWindow::fromGLFWKeyCode(int key) const
{
    // We are using the same key code table as GLFW
    return static_cast<gloperate::Key>(key);
}

gloperate::KeyModifier RenderWindow::fromGLFWModifier(int modifier) const
{
    //We are using the same modifier code table as GLFW
    return static_cast<gloperate::KeyModifier>(modifier);
}


} // namespace gloperate_glfw
