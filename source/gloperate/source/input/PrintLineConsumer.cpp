
#include <gloperate/input/PrintLineConsumer.h>
#include <gloperate/input/InputEvent.h>

#include <globjects/base/baselogging.h>

void gloperate::PrintLineConsumer::onEvent(gloperate::InputEvent * event)
{
    globjects::info() << event->asString();
}
