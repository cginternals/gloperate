#pragma once


#include <globjects/base/ref_ptr.h>
#include <gloperate/Painter.h>
#include <gloperate-qt/QtOpenGLWindowBase.h>


namespace gloperate_qt
{


class GLOPERATE_QT_API QtOpenGLWindow : public QtOpenGLWindowBase
{


public:
    QtOpenGLWindow();
    QtOpenGLWindow(const QSurfaceFormat & format);
    virtual ~QtOpenGLWindow();

    gloperate::Painter * painter() const;
    void setPainter(gloperate::Painter * painter);


protected:
    virtual void onInitialize();
    virtual void onResize(QResizeEvent * event);
    virtual void onPaint();


protected:
    globjects::ref_ptr<gloperate::Painter> m_painter;


};


} // namespace gloperate-qt
