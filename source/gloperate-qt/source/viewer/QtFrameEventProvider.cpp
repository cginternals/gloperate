#include <gloperate-qt/viewer/QtFrameEventProvider.h>

#include <gloperate/ext-includes-begin.h>
#include <QEvent>
#include <gloperate/ext-includes-end.h>

#include <gloperate/input/FrameEvent.h>

using namespace gloperate;

namespace gloperate_qt {

bool QtFrameEventProvider::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::UpdateRequest)
    {
        auto now = Clock::now();
        if (m_lastFrameTick != Clock::time_point())
        {
            auto delta = now - m_lastFrameTick;
            auto frameEvent = new FrameEvent(delta);
            passEventWithContext(obj, frameEvent);
        }
        m_lastFrameTick = now;
    }

    return QObject::eventFilter(obj, event);
}

} /* namespace gloperate_qt */
