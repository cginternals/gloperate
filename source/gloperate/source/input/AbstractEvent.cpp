#include <gloperate/input/AbstractEvent.h>


namespace gloperate
{

AbstractEvent::AbstractEvent()
{
}

AbstractEvent::~AbstractEvent()
{
}

SourceType AbstractEvent::sourceType() const
{
    return m_sourceType;
}

} // namespace gloperate
