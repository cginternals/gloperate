#include <gloperate-glfw/WindowEventHandlerBase.h>
#include <gloperate-glfw/events.h>


namespace gloperate_glfw
{


WindowEventHandlerBase::WindowEventHandlerBase()
{
}

WindowEventHandlerBase::~WindowEventHandlerBase()
{
}

void WindowEventHandlerBase::handleEvent(WindowEvent & event)
{
    if (!event.window())
        return;

    switch (event.type())
    {
        case WindowEvent::Resize:
            resizeEvent(static_cast<ResizeEvent&>(event));
            break;

        case WindowEvent::FrameBufferResize:
            framebufferResizeEvent(static_cast<ResizeEvent&>(event));
            break;

        case WindowEvent::Move:
            moveEvent(static_cast<MoveEvent&>(event));
            break;

        case WindowEvent::Paint:
            paintEvent(static_cast<PaintEvent&>(event));
            break;

        case WindowEvent::KeyPress:
            keyPressEvent(static_cast<KeyEvent&>(event));
            break;

        case WindowEvent::KeyRelease:
            keyReleaseEvent(static_cast<KeyEvent&>(event));
            break;

        case WindowEvent::MousePress:
            mousePressEvent(static_cast<MouseEvent&>(event));
            break;

        case WindowEvent::MouseRelease:
            mouseReleaseEvent(static_cast<MouseEvent&>(event));
            break;

        case WindowEvent::MouseMove:
            mouseMoveEvent(static_cast<MouseEvent&>(event));
            break;

        case WindowEvent::MouseEnter:

            break;

        case WindowEvent::MouseLeave:

            break;

        case WindowEvent::Scroll:
            scrollEvent(static_cast<ScrollEvent&>(event));
            break;

        case WindowEvent::Focus:
            focusEvent(static_cast<FocusEvent&>(event));
            break;

        case WindowEvent::Iconify:
            iconifyEvent(static_cast<IconifyEvent&>(event));
            break;

        case WindowEvent::Timer:
            timerEvent(static_cast<TimerEvent&>(event));
            break;

        default:
            break;
    }
}

void WindowEventHandlerBase::initialize(Window &)
{
}

void WindowEventHandlerBase::finalize(Window &)
{
}

void WindowEventHandlerBase::idle(Window &)
{
}

void WindowEventHandlerBase::resizeEvent(ResizeEvent &)
{
}

void WindowEventHandlerBase::framebufferResizeEvent(ResizeEvent &)
{
}

void WindowEventHandlerBase::moveEvent(MoveEvent &)
{
}

void WindowEventHandlerBase::paintEvent(PaintEvent &)
{
}

void WindowEventHandlerBase::keyPressEvent(KeyEvent &)
{
}

void WindowEventHandlerBase::keyReleaseEvent(KeyEvent &)
{
}

void WindowEventHandlerBase::mousePressEvent(MouseEvent &)
{
}

void WindowEventHandlerBase::mouseMoveEvent(MouseEvent &)
{
}

void WindowEventHandlerBase::mouseReleaseEvent(MouseEvent &)
{
}

void WindowEventHandlerBase::mouseEnterEvent(MouseEnterEvent &)
{
}

void WindowEventHandlerBase::mouseLeaveEvent(MouseLeaveEvent &)
{
}

void WindowEventHandlerBase::scrollEvent(ScrollEvent &)
{
}

void WindowEventHandlerBase::focusEvent(FocusEvent &)
{
}

void WindowEventHandlerBase::iconifyEvent(IconifyEvent &)
{
}

void WindowEventHandlerBase::timerEvent(TimerEvent &)
{
}


} // namespace gloperate_glfw
