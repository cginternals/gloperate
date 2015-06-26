/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate/input/AbstractEventProvider.h>

#include <gloperate/ext-includes-begin.h>
#include <QObject>
#include <QMouseEvent>
#include <gloperate/ext-includes-end.h>

namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtMouseEventProvider : public QObject, public gloperate::AbstractEventProvider
{
    Q_OBJECT

public:
    /**
    *  @brief
    *    Constructor
    */
    QtMouseEventProvider();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtMouseEventProvider();

    virtual bool eventFilter(QObject * obj, QEvent * event) override;

};

} // namespace gloperate_qt
