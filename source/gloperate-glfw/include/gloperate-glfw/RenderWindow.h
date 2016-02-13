
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
    *    Viewer context to which the window belongs (must NOT be null)
    *  @param[in] title
    *    Window title
    *  @param[in] width
    *    Window width (in pixels)
    *  @param[in] height
    *    Window height (in pixels)
    */
    RenderWindow(gloperate::ViewerContext * viewerContext,
                 const std::string & title = "gloperate",
                 int width  = 1280,
                 int height =  720);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();

    /**
    *  @brief
    *    Get viewer context
    *
    *  @return
    *    Viewer context (cannot be null)
    */
    gloperate::ViewerContext * viewerContext() const;

    /**
    *  @brief
    *    Get surface that is rendered into the window
    *
    *  @return
    *    Surface (cannot be null)
    */
    gloperate::Surface * surface() const;


protected:
    RenderWindow(gloperate::ViewerContext * viewerContext,
                 const std::string & title,
                 int width,
                 int height,
                 gloperate::Surface * surface);

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
    gloperate::ViewerContext * m_viewerContext; ///< Viewer context to which the window belongs (must NOT be null)
    gloperate::Surface       * m_surface;       ///< Surface that control the rendering on the window
    glm::ivec2                 m_deviceSize;    ///< Window size (real device pixels)
    glm::ivec2                 m_virtualSize;   ///< Window size (virtual pixel size)
};


} // namespace gloperate_glfw
