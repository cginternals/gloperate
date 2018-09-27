
#include <gloperate/input/PrintLineConsumer.h>

#include <cppassist/logging/logging.h>

#include <gloperate/input/InputEvent.h>


namespace gloperate
{


PrintLineConsumer::PrintLineConsumer(InputManager * inputManager)
: AbstractEventConsumer(inputManager)
{
}

PrintLineConsumer::~PrintLineConsumer()
{
}

void PrintLineConsumer::onEvent(gloperate::InputEvent * event)
{
    cppassist::info() << event->asString();
}


} // namespace gloperate
