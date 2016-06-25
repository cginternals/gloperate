
#pragma once


#include <gloperate-qt/base/OpenGLWindow.h>


namespace gloperate {
    class Environment;
    class Canvas;
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
    *  @param[in] environment
    *    Environment to which the window belongs (must NOT be null!)
    */
    RenderWindow(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderWindow();

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Gloperate environment (cannot be null)
    */
    gloperate::Environment * environment() const;

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
    gloperate::Environment * m_environment; ///< Gloperate environment to which the window belongs (must NOT be null)
    gloperate::Canvas      * m_canvas;      ///< Canvas that renders onto the window (must NOT be null)
};


} // namespace gloperate-qt
