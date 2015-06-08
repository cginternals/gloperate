/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate-qt/AbstractQtEventProvider.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QObject>
#include <QKeyEvent>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate/input/input.h>
#include <gloperate/input/KeyboardEvent.h>

namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtKeyEventProvider : public QObject, public gloperate_qt::AbstractQtEventProvider
{
    Q_OBJECT

public:
    /**
    *  @brief
    *    Constructor
    */
    explicit QtKeyEventProvider(QObject * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtKeyEventProvider();

    virtual bool eventFilter(QObject * obj, QEvent * event) override;

};

} // namespace gloperate_qt
