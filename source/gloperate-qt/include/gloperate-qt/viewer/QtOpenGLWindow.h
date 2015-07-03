
#pragma once


#include <memory>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <gloperate-qt/viewer/QtOpenGLWindowBase.h>
#include <gloperate-qt/viewer/TimePropagator.h>


namespace gloperate
{
    class ResourceManager;
}


namespace gloperate_qt
{


/**
*  @brief
*    Qt window that can be used for rendering with gloperate and OpenGL
*
*    This is the main rendering widget that displays a gloperate::Painter
*    within a Qt application. It contains only the rendering widget itself,
*    no other components. For a fully-fledged application template, see
*    gloperate_qt::Viewer.
*/
class GLOPERATE_QT_API QtOpenGLWindow : public QtOpenGLWindowBase
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] resourceManager
    *    Manager for loading/saving resources (i.e., textures, scenes)
    */
    QtOpenGLWindow(gloperate::ResourceManager & resourceManager);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] resourceManager
    *    Manager for loading/saving resources (i.e., textures, scenes)
    *  @param[in] format
    *    Surface format
    */
    QtOpenGLWindow(gloperate::ResourceManager & resourceManager, const QSurfaceFormat & format);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtOpenGLWindow();

    /**
    *  @brief
    *    Get used painter
    *
    *  @return
    *    Painter, can be nullptr
    */
    gloperate::Painter * painter() const;

    /**
    *  @brief
    *    Set used painter
    *
    *  @param[in] painter
    *    Painter, can be nullptr
    */
    void setPainter(gloperate::Painter * painter);


protected:
    virtual void onInitialize() override;
    virtual void onResize(QResizeEvent * event) override;
    virtual void onPaint() override;
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;


protected:
    gloperate::ResourceManager & m_resourceManager;
    gloperate::Painter * m_painter;                    ///< Currently used painter
    std::unique_ptr<TimePropagator> m_timePropagator;  ///< Time propagator for continous updates
};


} // namespace gloperate-qt
