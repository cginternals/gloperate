
#include <gloperate-qt/viewer/QtKeyEventProvider.h>

#include <gloperate/input/input.h>
#include <gloperate/input/KeyboardEvent.h>

#include <gloperate-qt/viewer/QtEventTransformer.h>


using namespace gloperate;


namespace gloperate_qt
{


QtKeyEventProvider::QtKeyEventProvider(QObject * parent)
:   QObject{ parent }
{
}

QtKeyEventProvider::~QtKeyEventProvider()
{
}

bool QtKeyEventProvider::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::KeyPress ||
        event->type() == QEvent::KeyRelease)
    {
        QKeyEvent * qKeyEvent = dynamic_cast<QKeyEvent*>(event);
        if (qKeyEvent) {
            auto eventType = QtEventTransformer::keyboardTypeFromQtType(qKeyEvent->type());
            auto key = QtEventTransformer::fromQtKeyCode(qKeyEvent->key(), qKeyEvent->modifiers());
            KeyboardEvent * keyEvent = new KeyboardEvent(eventType
                , key, qKeyEvent->nativeScanCode(), qKeyEvent->modifiers());
            passEventWithContext(obj, keyEvent);
            return false;
        }
    }
    return QObject::eventFilter(obj, event);
}


} // namespace gloperate_qt
