/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate/gloperate_api.h>
#include <globjects/base/ref_ptr.h>


namespace gloperate
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_API QtEventTransformer
{


public:
    /**
    *  @brief
    *    Constructor
    */
    QtEventTransformer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtEventTransformer();


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
