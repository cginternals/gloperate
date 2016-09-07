
#include <gloperate-glfw/WindowEventHandler.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <globjects/globjects.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractVirtualTimeCapability.h>
#include <gloperate/painter/AbstractInputCapability.h>
#include <gloperate/tools/ImageExporter.h>

#include <gloperate-glfw/Window.h>
#include <gloperate-glfw/events.h>


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
    case GLFW_MOUSE_BUTTON_1: return MouseButton::MouseButton1;
    case GLFW_MOUSE_BUTTON_2: return MouseButton::MouseButton2;
    case GLFW_MOUSE_BUTTON_3: return MouseButton::MouseButton3;
    case GLFW_MOUSE_BUTTON_4: return MouseButton::MouseButton4;
    case GLFW_MOUSE_BUTTON_5: return MouseButton::MouseButton5;
    case GLFW_MOUSE_BUTTON_6: return MouseButton::MouseButton6;
    case GLFW_MOUSE_BUTTON_7: return MouseButton::MouseButton7;
    case GLFW_MOUSE_BUTTON_8: return MouseButton::MouseButton8;
    default:                  return MouseButton::NoMouseButton;
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


WindowEventHandler::WindowEventHandler()
{
}

WindowEventHandler::~WindowEventHandler()
{
}

void WindowEventHandler::initialize(Window & window)
{
    globjects::init();
#ifndef N_DEBUG
    globjects::DebugMessage::enable(true);
#endif

    if (window.painter())
        window.painter()->initialize();
    }

void WindowEventHandler::framebufferResizeEvent(ResizeEvent & event)
{
    if (event.window()->painter()) {
        // Check if the painter supports the viewport capability
        AbstractViewportCapability * viewportCapability = event.window()->painter()->getCapability<AbstractViewportCapability>();

        if (viewportCapability)
        {
            // Resize painter
            viewportCapability->setViewport(0, 0, event.width(), event.height());
        }
    }
}

void WindowEventHandler::paintEvent(PaintEvent & event)
{
    if (event.window()->painter()) {
        // Call painter
        event.window()->painter()->paint();
    }
}

void WindowEventHandler::keyPressEvent(KeyEvent & event)
{
    if (event.key() == GLFW_KEY_F10)
    {
		if (ImageExporter::isApplicableTo(event.window()->painter()))
        {
			ImageExporter image(event.window()->painter(), event.window()->resourceManager());

            image.initialize();

            image.save("image.png");
        }
    }

    // Check for input capability
    if (event.window()->painter() && event.window()->painter()->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        event.window()->painter()->getCapability<gloperate::AbstractInputCapability>()->onKeyDown(
            fromGLFWKeyCode(event.key())
        );
    }
}

void WindowEventHandler::keyReleaseEvent(KeyEvent & event)
{
    // Check for input capability
    if (event.window()->painter() && event.window()->painter()->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        event.window()->painter()->getCapability<gloperate::AbstractInputCapability>()->onKeyUp(
            fromGLFWKeyCode(event.key())
        );
    }
}

void WindowEventHandler::mouseMoveEvent(MouseEvent & event)
{
    // Check for input capability
    if (event.window()->painter() && event.window()->painter()->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        event.window()->painter()->getCapability<gloperate::AbstractInputCapability>()->onMouseMove(
            event.x(),
            event.y()
        );
    }
}

void WindowEventHandler::mousePressEvent(MouseEvent & event)
{
    // Check for input capability
    if (event.window()->painter() && event.window()->painter()->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        event.window()->painter()->getCapability<gloperate::AbstractInputCapability>()->onMousePress(
            event.x(),
            event.y(),
            fromGLFWMouseButton(event.button())
        );
    }
    }

void WindowEventHandler::mouseReleaseEvent(MouseEvent & event)
{
    // Check for input capability
    if (event.window()->painter() && event.window()->painter()->supports<gloperate::AbstractInputCapability>()) {
        // Propagate event
        event.window()->painter()->getCapability<gloperate::AbstractInputCapability>()->onMouseRelease(
            event.x(),
            event.y(),
            fromGLFWMouseButton(event.button())
        );
    }
}

void WindowEventHandler::timerEvent(TimerEvent & event)
{
    if (!event.window()->painter())
        return;

    AbstractVirtualTimeCapability * timeCapability = event.window()->painter()->getCapability<AbstractVirtualTimeCapability>();

    if (timeCapability && timeCapability->enabled())
    {
        timeCapability->update(std::chrono::duration_cast<std::chrono::duration<float>>(event.elapsed()).count());
        event.window()->repaint();
    }
}


} // namespace gloperate_glfw
