
#include <globjects/logging.h>

#include <gloperate/input/MetaphorPrinter.h>
#include <gloperate/input/InputEvent.h>

namespace gloperate{

MetaphorPrinter::MetaphorPrinter()
    : m_metaphor( std::set<InputEvent::Type>{ InputEvent::Type::MouseMove , InputEvent::Type::MouseButtonPress}, &MetaphorPrinter::print, this )
{

}

void MetaphorPrinter::print(InputEvent * event)
{
    globjects::debug() << "Event: " << event->asString();

}

} // namespace gloperate
