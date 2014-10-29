#include "QtViewerMapping.h"

#include <QDebug>

#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>

using namespace gloperate;

QtViewerMapping::QtViewerMapping()
{
}

QtViewerMapping::~QtViewerMapping()
{
}

void QtViewerMapping::processEvent(AbstractEvent * event)
{
    if (event->sourceType() == gloperate::SourceType::Keyboard)
    {
        KeyboardEvent * keyEvent = dynamic_cast<KeyboardEvent*>(event);
        if (keyEvent)
        {
            if (keyEvent->key() == gloperate::KeyU)
            {
                qDebug() << "'U' was pressed!";
            }
        }
    }
    else if (event->sourceType() == gloperate::SourceType::Mouse)
    {
        MouseEvent * mouseEvent = dynamic_cast<MouseEvent*>(event);
        if (mouseEvent)
        {
            if (mouseEvent->button() == gloperate::MouseButton1)
            {
                qDebug() << "'MouseButton1' was pressed!";
            }
        }
    }
}

