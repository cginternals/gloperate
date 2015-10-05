
#pragma once

#include <gloperate/ext-includes-begin.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <gloperate/ext-includes-end.h>

#include <glm/glm.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/input/input.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>

#include <gloperate-qt/gloperate-qt_api.h>


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
    static gloperate::Key fromQtKeyCode(int key, int mods);
};


} // namespace gloperate_qt
