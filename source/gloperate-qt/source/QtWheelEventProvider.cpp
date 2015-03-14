/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate-qt/QtWheelEventProvider.h>

#include <gloperate/input/input.h>
#include <gloperate/input/WheelEvent.h>

#include <gloperate-qt/QtEventTransformer.h>

using namespace gloperate;

namespace gloperate_qt
{

/**
*  @brief
*    Constructor
*/
QtWheelEventProvider::QtWheelEventProvider()
{
}

/**
*  @brief
*    Destructor
*/
QtWheelEventProvider::~QtWheelEventProvider()
{
}

bool QtWheelEventProvider::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Wheel)
    {
        QWheelEvent * qWheelEvent = dynamic_cast<QWheelEvent*>(event);
        if (qWheelEvent) {
            auto position = QtEventTransformer::fromQPoint(qWheelEvent->pos());
            auto qAngleDelta = QtEventTransformer::fromQPoint(qWheelEvent->angleDelta());
            auto angleDelta = toDegrees(qAngleDelta);
            auto pixelDelta = QtEventTransformer::fromQPoint(qWheelEvent->pixelDelta());
            WheelEvent * wheelEvent = new WheelEvent(position, angleDelta, pixelDelta);
            passEventWithContext(obj, wheelEvent);
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

glm::vec2 QtWheelEventProvider::toDegrees(const glm::ivec2 & eigthsOfADegree)
{
    return glm::vec2(eigthsOfADegree) / 8.f;
}

} // namespace gloperate_qt
