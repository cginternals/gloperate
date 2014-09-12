#include <gloperate-glfw/WindowEventHandler.h>
#include <globjects/globjects.h>
#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/events.h>
#include <gloperate/Viewport.h>

#include <gloperate/capabilities/AbstractViewportCapability.h>

using namespace gloperate;
namespace gloperate_glfw
{


WindowEventHandler::WindowEventHandler()
{
}

WindowEventHandler::~WindowEventHandler()
{
}

Painter * WindowEventHandler::painter() const
{
    return m_painter;
}

void WindowEventHandler::setPainter(Painter * painter)
{
    m_painter = painter;
}

void WindowEventHandler::initialize(gloperate_glfw::Window & /*window*/)
{
    // Initialize globjects
    globjects::init();
    IF_DEBUG(globjects::DebugMessage::enable(true);)

    // Initialize painter
    if (m_painter) {
        m_painter->initialize();
    }
}

void WindowEventHandler::idle(Window & window)
{
    // Continuous repaint
    window.repaint();
}

void WindowEventHandler::framebufferResizeEvent(ResizeEvent & event)
{
    if (m_painter) {
        // Resize painter
        AbstractViewportCapability * viewportCapability = m_painter->getCapability<AbstractViewportCapability>();

        if (viewportCapability)
        {
            // Resize painter
            viewportCapability->setViewport(Viewport(0, 0, event.width(), event.height()));
        }
    }
}

void WindowEventHandler::paintEvent(PaintEvent & /*event*/)
{
    if (m_painter) {
        // Call painter
        m_painter->paint();
    }
}

void WindowEventHandler::keyPressEvent(KeyEvent & /*event*/)
{
}


} // namespace gloperate_glfw
