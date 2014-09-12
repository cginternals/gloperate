/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/VirtualTimeCapability.h>
#include <cassert>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
VirtualTimeCapability::VirtualTimeCapability()
: m_active(true)
, m_duration(2.0f * 3.141592654f)
, m_time(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
VirtualTimeCapability::~VirtualTimeCapability()
{
}

bool VirtualTimeCapability::isActive() const
{
    return m_active;
}

void VirtualTimeCapability::setActive(bool active)
{
    m_active = active;

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
