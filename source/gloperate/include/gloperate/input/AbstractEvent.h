#pragma once

#include <gloperate/gloperate_api.h>


namespace gloperate
{

enum class EventType
{
    Default
,   Press
,   Release
,   Move
,   Scroll
};

enum class SourceType
{
    Keyboard
,   Mouse
,   Timer
};

class GLOPERATE_API AbstractEvent
{
public:
    virtual ~AbstractEvent();

    EventType eventType() const;
    SourceType sourceType() const;

protected:
    AbstractEvent(EventType type);

protected:
    EventType m_eventType;
    SourceType m_sourceType;

};

} // namespace gloperate
