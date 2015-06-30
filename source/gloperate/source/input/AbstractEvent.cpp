
#include <gloperate/input/AbstractEvent.h>


namespace gloperate
{


AbstractEvent::AbstractEvent()
: m_sourceType(EventType::Unknown)
{
}

AbstractEvent::~AbstractEvent()
{
}

EventType AbstractEvent::sourceType() const
{
    return m_sourceType;
}


} // namespace gloperate
