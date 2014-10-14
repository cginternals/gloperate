#pragma once


#include <gloperate-qt/qt-includes-begin.h>
#include <QWindow>
#include <QScopedPointer>
#include <QSurfaceFormat>
#include <gloperate-qt/qt-includes-end.h>
#include <gloperate-qt/gloperate-qt_api.h>


class QOpenGLContext;


namespace gloperate_qt
{


class GLOPERATE_QT_API QtOpenGLWindowBase : public QWindow
{


public:
    static QSurfaceFormat defaultFormat();


public:
    QtOpenGLWindowBase();
    QtOpenGLWindowBase(const QSurfaceFormat & format);
    virtual ~QtOpenGLWindowBase();

    QOpenGLContext * context() const;
    void updateGL();


protected:
    void initialize();
    void resize(QResizeEvent * event);
    void paint();


protected:
    virtual void onInitialize();
    virtual void onResize(QResizeEvent * event);
    virtual void onPaint();

    virtual bool event(QEvent * event) override;
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void exposeEvent(QExposeEvent * event) override;
    virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);

    void makeCurrent();
    void doneCurrent();
protected:
    QScopedPointer<QOpenGLContext> m_context;
    bool                           m_initialized;
    bool                           m_updatePending;


};


} // namespace gloperate-qt
