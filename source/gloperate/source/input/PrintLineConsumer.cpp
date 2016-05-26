
#include <gloperate/input/PrintLineConsumer.h>

#include <cppassist/logging/logging.h>

#include <gloperate/input/InputEvent.h>


using namespace cppassist;


namespace gloperate
{


void PrintLineConsumer::onEvent(gloperate::InputEvent * event)
{
    info() << event->asString();
}


} // namespace gloperate
