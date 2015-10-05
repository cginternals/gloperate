
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Event type
*
*    Describes the type of an event, such as
*    keyboard, mouse, or timer events. This
*    is used by the event provider system.
*
*  @see
*    AbstractEventProvider
*/
enum class EventType : char
{
    Unknown
,   Keyboard
,   Mouse
,   Timer
,   Wheel
,   Frame
};


/**
*  @brief
*    Abstract base class for events
*
*    Is subclassed to represent specific events,
*    such as keyboard, mouse, or timer events.
*/
class GLOPERATE_API AbstractEvent
{
public:
    virtual ~AbstractEvent();

    EventType sourceType() const;


protected:
    AbstractEvent();


protected:
    EventType m_sourceType;
};


} // namespace gloperate
