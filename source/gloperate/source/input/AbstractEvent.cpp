#include <gloperate/input/AbstractEvent.h>


namespace gloperate
{

AbstractEvent::AbstractEvent(EventType type)
: m_eventType(type)
{
}

AbstractEvent::~AbstractEvent()
{
}

EventType AbstractEvent::eventType() const
{
    return m_eventType;
}

SourceType AbstractEvent::sourceType() const
{
    return m_sourceType;
}

} // namespace gloperate
