
#pragma once


#include <globjects/base/Referenced.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


namespace gloperate_glfw
{


class Window;
class WindowEvent;
class KeyEvent;
class MouseEvent;
class MouseEnterEvent;
class MouseLeaveEvent;
class ScrollEvent;
class ResizeEvent;
class PaintEvent;
class FocusEvent;
class IconifyEvent;
class MoveEvent;
class TimerEvent;


/**
*  @brief
*    Objects of this class can be attached to a window to handle events
*
*  @remarks
*    The window's context is made current before calling any methods and done current afterwards.
*/
class GLOPERATE_GLFW_API WindowEventHandlerBase : public globjects::Referenced
{
public:
    WindowEventHandlerBase();
    virtual ~WindowEventHandlerBase();

    virtual void handleEvent(WindowEvent & event);

    /**
    *  @brief
    *    Initialize event handler
    *
    *  @remarks
    *    'initialize' is called only once when starting to run a window.
    */
    virtual void initialize(Window & window);
    
    /**
    *  @brief
    *    Finalize event handler
    *
    *  @remarks
    *    'finalize' is called only once, just before a window returns from running.
    */
    virtual void finalize(Window & window);

    /**
    *  @brief
    *    Called on idle
    *
    *  @remarks
    *    'idle' is called when no events were queued in the current iteration of the main loop.
    */
    virtual void idle(Window & window);


protected:
    virtual void resizeEvent(ResizeEvent & event);
    virtual void framebufferResizeEvent(ResizeEvent & event);

    virtual void moveEvent(MoveEvent & event);

    /**
    *  @brief
    *    Paint window
    *
    *  @remarks
    *    'swapBuffers' gets called afterwards.
    */
    virtual void paintEvent(PaintEvent & event);

    virtual void keyPressEvent(KeyEvent & event);
    virtual void keyReleaseEvent(KeyEvent & event);

    virtual void mousePressEvent(MouseEvent & event);
    virtual void mouseMoveEvent(MouseEvent & event);
    virtual void mouseReleaseEvent(MouseEvent & event);

    virtual void mouseEnterEvent(MouseEnterEvent & event);
    virtual void mouseLeaveEvent(MouseLeaveEvent & event);

    virtual void scrollEvent(ScrollEvent & event);

    virtual void focusEvent(FocusEvent & event);
    virtual void iconifyEvent(IconifyEvent & event);

    virtual void timerEvent(TimerEvent & event);
};


} // namespace gloperate_glfw
