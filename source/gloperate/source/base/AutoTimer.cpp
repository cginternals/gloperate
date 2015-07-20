
#include <gloperate/base/AutoTimer.h>

#include <cmath>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>

#include <globjects/base/baselogging.h>

#include <gloperate/base/ChronoTimer.h>


namespace 
{
    // use number of digits to retrieve exp in 10^(3 exp)
    const char units[3] = { 'n', 'u', 'm' };
}


namespace gloperate
{


int AutoTimer::s_numActiveInstances = 0;


AutoTimer::AutoTimer(const std::string & info)
:   m_info(info)
,   m_index(++s_numActiveInstances)
,   m_timer(new ChronoTimer(false))
{
    m_timer->start();
}

AutoTimer::~AutoTimer()
{
    m_timer->pause();

    ChronoTimer::Duration elapsed = m_timer->elapsed();
    //double delta(static_cast<double>(m_timer->elapsed()));

    using nano = std::chrono::nanoseconds;

    nano delta = std::chrono::duration_cast<nano>(elapsed);
    const unsigned int unitPrecision = std::min(3u, static_cast<unsigned int>(ceil(log10(static_cast<double>(delta.count())) / 3.0)));

    std::string unit = "s";

    if (unitPrecision < sizeof(units))
        unit.insert(0, 1, units[unitPrecision]);

    // shorten the time to nearest time unit
    double deltaf = static_cast<double>(delta.count()) / pow(1000.0, unitPrecision);

    globjects::debug() << m_info << " took "
        << std::setprecision(4) << deltaf << unit
        << " (timer_" << std::setfill('0') << std::setw(2) << m_index << ").";

    --s_numActiveInstances;
}


} // namespace gloperate
