#pragma once

#include <gloperate/gloperate_api.h>


namespace gloperate
{

enum class EventType : char
{
    Default
,   Press
,   Release
,   Move
,   Scroll
};

enum class SourceType : char
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
