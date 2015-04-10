#include <gloperate/input/AbstractEvent.h>


namespace gloperate
{

AbstractEvent::AbstractEvent()
: m_sourceType(SourceType::Unknown)
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
