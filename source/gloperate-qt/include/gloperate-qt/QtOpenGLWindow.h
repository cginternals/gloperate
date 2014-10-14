#pragma once


#include <QScopedPointer>
#include <globjects/base/ref_ptr.h>
#include <gloperate/Painter.h>
#include <gloperate-qt/QtOpenGLWindowBase.h>
#include <gloperate-qt/TimePropagator.h>


namespace gloperate
{

class ResourceManager;

}

namespace gloperate_qt
{


class GLOPERATE_QT_API QtOpenGLWindow : public QtOpenGLWindowBase
{


public:
    QtOpenGLWindow(gloperate::ResourceManager & resourceManager);
    QtOpenGLWindow(gloperate::ResourceManager & resourceManager, const QSurfaceFormat & format);
    virtual ~QtOpenGLWindow();

    gloperate::Painter * painter() const;
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
    globjects::ref_ptr<gloperate::Painter> m_painter;	      /**< Currently used painter */
    QScopedPointer<TimePropagator>         m_timePropagator;  /**< Time propagator for continous updates */


};


} // namespace gloperate-qt
