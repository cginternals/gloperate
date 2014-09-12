/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <QScopedPointer>
#include <globjects/base/ref_ptr.h>
#include <gloperate/Painter.h>
#include <gloperate-qt/QtOpenGLWindowBase.h>
#include <gloperate-qt/TimePropagator.h>


namespace gloperate_qt
{


/**
*  @brief
*    Qt window that can be used for rendering with gloperate and OpenGL
*/
class GLOPERATE_QT_API QtOpenGLWindow : public QtOpenGLWindowBase
{


public:
    /**
    *  @brief
    *    Constructor
    */
    QtOpenGLWindow();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] format
    *    Surface format
    */
    QtOpenGLWindow(const QSurfaceFormat & format);

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


protected:
    globjects::ref_ptr<gloperate::Painter> m_painter;	      /**< Currently used painter */
    QScopedPointer<TimePropagator>         m_timePropagator;  /**< Time propagator for continous updates */


};


} // namespace gloperate-qt
