/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate-qt/AbstractQtEventProvider.h>

#include <glm/glm.hpp>

#include <gloperate-qt/qt-includes-begin.h>
#include <QObject>
#include <QWheelEvent>
#include <gloperate-qt/qt-includes-end.h>

namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtWheelEventProvider : public QObject, public gloperate_qt::AbstractQtEventProvider
{
    Q_OBJECT

public:
    /**
    *  @brief
    *    Constructor
    */
    QtWheelEventProvider();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtWheelEventProvider();

    virtual bool eventFilter(QObject * obj, QEvent * event) override;

protected:
    static glm::vec2 toDegrees(const glm::ivec2 & eigthsOfADegree);
};

} // namespace gloperate_qt
