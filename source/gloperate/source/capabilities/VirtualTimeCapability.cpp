#include <gloperate/capabilities/VirtualTimeCapability.h>
#include <cassert>


namespace gloperate
{


VirtualTimeCapability::VirtualTimeCapability()
: m_enabled(true)
, m_duration(2.0f * 3.141592654f)
, m_time(0.0f)
{
}

VirtualTimeCapability::~VirtualTimeCapability()
{
}

bool VirtualTimeCapability::enabled() const
{
    return m_enabled;
}

void VirtualTimeCapability::setEnabled(bool enabled)
{
    m_enabled = enabled;

    setChanged(true);
}

float VirtualTimeCapability::time() const
{
    return m_time;
}

void VirtualTimeCapability::setLoopDuration(float duration)
{
    assert(duration > 0.0f);

    m_duration = duration;

    normalizeTime();
}

void VirtualTimeCapability::update(float delta)
{
    m_time += delta;

    setChanged(true);

    normalizeTime();
}

void VirtualTimeCapability::normalizeTime()
{
    while (m_time > m_duration)
    {
        m_time -= m_duration;

        setChanged(true);
    }
}


} // namespace gloperate
