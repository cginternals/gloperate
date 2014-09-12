#include <gloperate/capabilities/VirtualTimeCapability.h>

#include <cassert>

namespace gloperate
{

VirtualTimeCapability::VirtualTimeCapability()
: m_duration(2.0f * 3.141592654f)
, m_time(0.0f)
{
}

VirtualTimeCapability::~VirtualTimeCapability()
{
}

void VirtualTimeCapability::setLoopDuration(float duration)
{
    assert(duration > 0.0f);

    m_duration = duration;

    normalizeTime();
}

float VirtualTimeCapability::time() const
{
    return m_time;
}

void VirtualTimeCapability::update(float delta)
{
    m_time += delta;

    normalizeTime();
}

void VirtualTimeCapability::normalizeTime()
{
    while (m_time > m_duration)
    {
        m_time -= m_duration;
    }
}

} // namespace gloperate
