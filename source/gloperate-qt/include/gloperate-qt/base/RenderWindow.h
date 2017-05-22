
#pragma once


#include <memory>

#include <gloperate-qt/base/OpenGLWindow.h>


namespace gloperate
{
    class Environment;
    class Canvas2;
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
    *    Get canvas
    *
    *  @return
    *    Canvas that renders onto the window (cannot be null)
    */
    gloperate::Canvas2 * canvas() const;


protected:
    // Virtual OpenGLWindow functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onResize(const QSize & deviceSize, const QSize & virtualSize) override;
    virtual void onPaint() override;
    virtual void onTimer() override;

    // Qt event functions
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;


protected:
    gloperate::Environment            * m_environment; ///< Gloperate environment to which the window belongs (must NOT be null)
    std::unique_ptr<gloperate::Canvas2> m_canvas;      ///< Canvas that renders onto the window (never null)
};


} // namespace gloperate-qt
