#pragma once

#include <chrono>
#include <cstdint>
#include <vector>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class GLOPERATE_API FrameCounter
{
public:
    FrameCounter();

    void countFrame();

    double computeFps() const;
    std::uint64_t numFrames() const;

private:
    using Clock = std::chrono::high_resolution_clock;
    using FrameTime = std::uint64_t;

private:
    Clock::time_point m_lastFrame;
    std::uint64_t m_counter;
    std::vector<FrameTime> m_window;
};

}  // namespace gloperate
