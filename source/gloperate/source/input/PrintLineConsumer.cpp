
#include <gloperate/input/PrintLineConsumer.h>

#include <globjects/base/baselogging.h>

#include <gloperate/input/InputEvent.h>


namespace gloperate
{


void PrintLineConsumer::onEvent(gloperate::InputEvent * event)
{
    globjects::info() << event->asString();
}


} // namespace gloperate
