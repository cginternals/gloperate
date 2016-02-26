
#pragma once


#include <gloperate-qt/viewer/OpenGLWindow.h>


namespace gloperate {
    class ViewerContext;
    class RenderSurface;
    class Stage;
}


namespace gloperate_qt
{


/**
*  @brief
*    Window that renders a gloperate scene
*/
class GLOPERATE_QT_API RenderWindow : public OpenGLWindow
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the window belongs (must NOT be null!)
    *  @param[in] renderStage
    *    Stage that renders into the surface (can be null)
    */
    RenderWindow(
        gloperate::ViewerContext * viewerContext
      , gloperate::Stage * renderStage = nullptr
    );

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
    RenderWindow(gloperate::ViewerContext * viewerContext,
                 gloperate::RenderSurface * surface);

    // Virtual OpenGLWindow functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onResize(const QSize & deviceSize, const QSize & virtualSize) override;
    virtual void onPaint() override;

    // Qt event functions
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;


protected:
    gloperate::ViewerContext * m_viewerContext; ///< Viewer context to which the window belongs (must NOT be null)
    gloperate::RenderSurface * m_surface;       ///< Surface that renders on the window (must NOT be null)
};


} // namespace gloperate-qt
