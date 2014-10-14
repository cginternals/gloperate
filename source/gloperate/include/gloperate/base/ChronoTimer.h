#pragma once

#include <chrono>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class GLOPERATE_API ChronoTimer
{
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
public:
    using Duration = std::chrono::duration<long long, std::nano>;

    ChronoTimer(bool start = true, bool autoUpdate = true);
    virtual ~ChronoTimer();

    void setAutoUpdating(const bool autoUpdate);
    bool autoUpdating() const;

    bool paused() const;

    void update() const;
    Duration elapsed() const;

    void start();
    void pause();
    void stop();
    void reset();

protected:
    bool m_paused;
    bool m_auto;

protected:
    time_point m_t0;
    time_point m_tp; // time_point of last pausing

    mutable time_point m_t1;

    Duration m_offset;
    mutable Duration m_elapsed;
};

} // namespace gloperate
