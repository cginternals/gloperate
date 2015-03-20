/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once

#include <gloperate-qt/gloperate-qt_api.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <gloperate-qt/qt-includes-end.h>

#include <glm/glm.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/input/input.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>

namespace gloperate_qt
{


/**
*  @brief
*    Class that transforms Qt events to gloperate events
*/
class GLOPERATE_QT_API QtEventTransformer
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

    static gloperate::MouseEvent::Type mouseTypeFromQtType(QEvent::Type type);
    static gloperate::KeyboardEvent::Type keyboardTypeFromQtType(QEvent::Type type);
    static glm::ivec2 fromQPoint(const QPoint point);
    static gloperate::MouseButton fromQtMouseButton(Qt::MouseButton button);
    static gloperate::MouseButton fromQtMouseButtons(Qt::MouseButtons button);
    static gloperate::Key fromQtKeyCode(int key, int mods);
    static gloperate::KeyModifier fromQtKeyboardModifiers(Qt::KeyboardModifiers modifiers);

};


} // namespace gloperate_qt
