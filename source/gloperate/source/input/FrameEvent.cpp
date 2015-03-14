#include <gloperate/input/FrameEvent.h>

namespace gloperate
{

FrameEvent::FrameEvent(Clock::duration deltaTime)
: m_deltaTime(deltaTime)
{
}

FrameEvent::Clock::duration FrameEvent::deltaTime() const
{
    return m_deltaTime;
}

} /* namespace gloperate */
