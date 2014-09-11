#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QObject>
#include <QTimer>
#include <QScopedPointer>
#include <QWidget>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate/capabilities/VirtualTimeCapability.h>
#include <gloperate/ChronoTimer.h>
#include <gloperate-qt/QtOpenGLWindowBase.h>
#include <gloperate-qt/gloperate-qt_api.h>

namespace gloperate_qt
{

class GLOPERATE_QT_API TimePropagator : public QObject
{
    Q_OBJECT
public:
    TimePropagator(gloperate_qt::QtOpenGLWindowBase* window, gloperate::VirtualTimeCapability * capability);
public slots:
    virtual void update();
protected:
    gloperate_qt::QtOpenGLWindowBase * m_window;
    QScopedPointer<QTimer> m_timer;
    gloperate::ChronoTimer m_time;
    gloperate::VirtualTimeCapability * m_capability;
};

} // namespace gloperate_qt
