
#pragma once


#include <gloperate-glfw/WindowEventHandlerBase.h>


namespace gloperate_glfw
{


class GLOPERATE_GLFW_API WindowEventHandler : public WindowEventHandlerBase
{
public:
    WindowEventHandler();
    virtual ~WindowEventHandler();

    virtual void initialize(gloperate_glfw::Window & window) override;


protected:
    virtual void framebufferResizeEvent(ResizeEvent & event) override;

    virtual void paintEvent(PaintEvent & event) override;

    virtual void keyPressEvent(KeyEvent & event) override;
    virtual void keyReleaseEvent(KeyEvent & event) override;

    virtual void mouseMoveEvent(MouseEvent & event) override;
    virtual void mousePressEvent(MouseEvent & event) override;
    virtual void mouseReleaseEvent(MouseEvent & event) override;

    virtual void timerEvent(TimerEvent & event) override;
};


} // namespace gloperate_glfw
