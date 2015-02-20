#include <gloperate/tools/FrameCounter.h>

namespace gloperate
{

namespace
{

const int k_windowSize = 25;

}  // anonymous namespace

FrameCounter::FrameCounter()
    : m_counter(0)
, m_window(k_windowSize)
{
}

void FrameCounter::countFrame()
{
    auto now = Clock::now();

    if (m_counter > 0)
    {
        FrameTime us = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastFrame).count();
        m_window[m_counter % k_windowSize] = us;
    }

    m_lastFrame = now;
    ++m_counter;
}

double FrameCounter::computeFps() const
{
    FrameTime sum = 0;
    for (auto frame : m_window) {
        sum += frame;
    }

    if (sum > 0) {
        return k_windowSize / (sum / 1000000.0);
    }

    return 0.0;
}

std::uint64_t FrameCounter::numFrames() const
{
    return m_counter;
}

}  // namespace gloperate
