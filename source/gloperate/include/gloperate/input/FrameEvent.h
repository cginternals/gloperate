#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/input/AbstractEvent.h>

#include <chrono>

namespace gloperate {

class GLOPERATE_API FrameEvent: public AbstractEvent
{
public:
    using Clock = std::chrono::high_resolution_clock;

public:
    explicit FrameEvent(Clock::duration deltaTime);

    Clock::duration deltaTime() const;

protected:
    Clock::duration m_deltaTime;
};

} /* namespace gloperate */
