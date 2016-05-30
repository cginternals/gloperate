
#pragma once


#include <glm/glm.hpp>

#include <gloperate-glfw/viewer/Window.h>


namespace gloperate {
    class ViewerContext;
    class RenderSurface;
    class Stage;
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
    */
    RenderWindow(gloperate::ViewerContext * viewerContext);

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
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the window (can be null)
    */
    gloperate::Stage * renderStage() const;

     /**
    *  @brief
    *    Get render surface
    *
    *  @return
    *    Render surface that is rendered into
    */
    gloperate::RenderSurface * renderSurface() const;

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the window (can be null)
    *
    *  @remarks
    *    When setting a new render stage, the old render stage is destroyed.
    *    The window takes ownership over the stage.
    */
    void setRenderStage(gloperate::Stage * stage);


protected:
    // Virtual Window functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
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


protected:
    gloperate::ViewerContext * m_viewerContext; ///< Viewer context to which the window belongs (must NOT be null)
    gloperate::RenderSurface * m_surface;       ///< Surface that control the rendering on the window (must NOT be null)
    glm::ivec2                 m_deviceSize;    ///< Window size (real device pixels)
    glm::ivec2                 m_virtualSize;   ///< Window size (virtual pixel size)
};


} // namespace gloperate_glfw
