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
#include <QMouseEvent>
#include <gloperate-qt/qt-includes-end.h>

#include <glm/vec2.hpp>

namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtMouseEventProvider : public QObject, public gloperate_qt::AbstractQtEventProvider
{
    Q_OBJECT

public:
    /**
    *  @brief
    *    Constructor
    */
    explicit QtMouseEventProvider(QObject * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtMouseEventProvider();

    virtual bool eventFilter(QObject * obj, QEvent * event) override;

protected:
    glm::ivec2 m_lastPos;
};

} // namespace gloperate_qt
