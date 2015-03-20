/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-qt/QtMouseEventProvider.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <QWindow>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate/input/input.h>
#include <gloperate/input/MouseEvent.h>

#include <gloperate-qt/QtEventTransformer.h>

using namespace gloperate;

namespace gloperate_qt
{

/**
*  @brief
*    Constructor
*/
QtMouseEventProvider::QtMouseEventProvider()
{
}

/**
*  @brief
*    Destructor
*/
QtMouseEventProvider::~QtMouseEventProvider()
{
}

bool QtMouseEventProvider::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseMove)
    {
        auto qMouseEvent = dynamic_cast<QMouseEvent*>(event);
        auto window = dynamic_cast<QWindow*>(obj);
        if (qMouseEvent && window) {
            auto eventType = QtEventTransformer::mouseTypeFromQtType(qMouseEvent->type());
            auto position = QtEventTransformer::fromQPoint(qMouseEvent->pos());
            auto button = QtEventTransformer::fromQtMouseButton(qMouseEvent->button());
            auto buttonMask = QtEventTransformer::fromQtMouseButtons(qMouseEvent->buttons());
            auto modifiers = QtEventTransformer::fromQtKeyboardModifiers(qMouseEvent->modifiers());
            auto mouseEvent =
                    new MouseEvent(eventType,
                                      position,
                                      m_lastPos,
                                      { window->width(), window->height() },
                                      button,
                                      buttonMask,
                                      modifiers);
            m_lastPos = position;
            passEventWithContext(obj, mouseEvent);
            return false;
        }
    }

    if (event->type() == QEvent::Enter ||
        event->type() == QEvent::Leave)
    {
        auto qEnterEvent = dynamic_cast<QEnterEvent*>(event);
        if (qEnterEvent)
        {
            m_lastPos = QtEventTransformer::fromQPoint(qEnterEvent->pos());
        }

        auto eventType = QtEventTransformer::mouseTypeFromQtType(event->type());
        auto mouseEvent =
            new MouseEvent(eventType, glm::ivec2(), glm::ivec2(), glm::ivec2(), MouseButton::NoMouseButton, MouseButton::NoMouseButton, KeyModifier::ModNone);
        passEventWithContext(obj, mouseEvent);
        return false;
    }
    return QObject::eventFilter(obj, event);
}

} // namespace gloperate_qt
