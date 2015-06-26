
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
    case WindowEvent::Type::Resize:
        resizeEvent(static_cast<ResizeEvent &>(event));
        break;

    case WindowEvent::Type::FrameBufferResize:
        framebufferResizeEvent(static_cast<ResizeEvent &>(event));
        break;

    case WindowEvent::Type::Move:
        moveEvent(static_cast<MoveEvent &>(event));
        break;

    case WindowEvent::Type::Paint:
        paintEvent(static_cast<PaintEvent &>(event));
        break;

    case WindowEvent::Type::KeyPress:
        keyPressEvent(static_cast<KeyEvent &>(event));
        break;

    case WindowEvent::Type::KeyRelease:
        keyReleaseEvent(static_cast<KeyEvent &>(event));
        break;

    case WindowEvent::Type::MousePress:
        mousePressEvent(static_cast<MouseEvent &>(event));
        break;

    case WindowEvent::Type::MouseRelease:
        mouseReleaseEvent(static_cast<MouseEvent &>(event));
        break;

    case WindowEvent::Type::MouseMove:
        mouseMoveEvent(static_cast<MouseEvent &>(event));
        break;

    case WindowEvent::Type::MouseEnter:
        mouseEnterEvent(static_cast<MouseEnterEvent &>(event));
        break;

    case WindowEvent::Type::MouseLeave:
        mouseLeaveEvent(static_cast<MouseLeaveEvent &>(event));
        break;

    case WindowEvent::Type::Scroll:
        scrollEvent(static_cast<ScrollEvent &>(event));
        break;

    case WindowEvent::Type::Focus:
        focusEvent(static_cast<FocusEvent &>(event));
        break;

    case WindowEvent::Type::Iconify:
        iconifyEvent(static_cast<IconifyEvent &>(event));
        break;

    case WindowEvent::Type::Timer:
        timerEvent(static_cast<TimerEvent &>(event));
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
