
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <QTimer>
#include <QScopedPointer>
#include <QWidget>
#include <gloperate/ext-includes-end.h>

#include <gloperate/painter/AbstractVirtualTimeCapability.h>
#include <gloperate/base/ChronoTimer.h>

#include <gloperate-qt/viewer/QtOpenGLWindowBase.h>


namespace gloperate_qt
{


/**
*  @brief
*    Tool class to propagate continuous time updates to a window
*
*  @remarks
*    This class is used in a window to allow for continuous updates (e.g., to
*    implement animations). It take a VirtualTimeCapability of a painter to
*    propagate the time change to the painter, and automatically triggers
*    an update of the window containing the painter.
*/
class GLOPERATE_QT_API TimePropagator : public QObject
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] window
    *    Window that is updated when the timer has elapsed
    *  @param[in] capability
    *    VirtualTimeCapability that is informed about the time change
    */
    TimePropagator(gloperate_qt::QtOpenGLWindowBase * window);
    
    void setCapability(gloperate::AbstractVirtualTimeCapability * capability);


protected slots:
    /**
    *  @brief
    *    Called by the timer when the timer has elapsed
    */
    void update();


protected:
    gloperate_qt::QtOpenGLWindowBase         * m_window;     /**< Window that is updated when the timer has elapsed */
    gloperate::AbstractVirtualTimeCapability * m_capability; /**< VirtualTimeCapability that is informed about the time change */
    QScopedPointer<QTimer> m_timer; /**< Qt timer for continuous updates */
    gloperate::ChronoTimer m_time;  /**< Time measurement */
};


} // namespace gloperate_qt
