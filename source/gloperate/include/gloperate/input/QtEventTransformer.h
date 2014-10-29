/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <gloperate-qt/qt-includes-end.h>

#include <glm/glm.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/input/input.h>
#include <gloperate/input/AbstractEvent.h>

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

    static AbstractEvent * transformEvent(QEvent * event);

    static EventType fromQtType(QEvent::Type type);
    static glm::ivec2 fromQPoint(const QPoint point);
    static MouseButton fromQtMouseButton(Qt::MouseButton button);
    static Key fromQtKeyCode(int key, int mods);


};


} // namespace gloperate
