
#pragma once


#include <glm/glm.hpp>

#include <gloperate-glfw/Window.h>


namespace gloperate {
    class ViewerContext;
    class Surface;
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
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    RenderWindow(gloperate::ViewerContext * viewerContext);

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
    gloperate::Surface * m_surface;
    glm::ivec2           m_deviceSize;
    glm::ivec2           m_virtualSize;
};


} // namespace gloperate_glfw
