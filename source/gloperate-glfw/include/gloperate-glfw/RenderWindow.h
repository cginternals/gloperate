
#pragma once


#include <gloperate-glfw/Window.h>


namespace gloperate {
    class RenderHandler;
}


namespace gloperate_glfw
{


/**
*  @brief
*    Default rendering window for gloperate scenes
*/
class GLOPERATE_GLFW_API RenderWindow : public Window
{
public:
    /**
    *  @brief
    *    Constructor
    */
    RenderWindow();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();


protected:
    // Virtual Window functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onIdle() override;
    virtual void onResize(ResizeEvent & event) override;
    virtual void onFramebufferResize(ResizeEvent & event) override;
    virtual void onMove(MoveEvent & event) override;
    virtual void onPaint(PaintEvent & event) override;
    virtual void onKeyPress(KeyEvent & event) override;
    virtual void onKeyRelease(KeyEvent & event) override;
    virtual void onMousePress(MouseEvent & event) override;
    virtual void onMouseRelease(MouseEvent & event) override;
    virtual void onMouseMove(MouseEvent & event) override;
    virtual void onMouseEnter(MouseEnterEvent & event) override;
    virtual void onMouseLeave(MouseLeaveEvent & event) override;
    virtual void onScroll(ScrollEvent & event) override;
    virtual void onFocus(FocusEvent & event) override;
    virtual void onIconify(IconifyEvent & event) override;
    virtual void onTimer(TimerEvent & event) override;


protected:
    gloperate::RenderHandler * m_renderHandler;
};


} // namespace gloperate_glfw
