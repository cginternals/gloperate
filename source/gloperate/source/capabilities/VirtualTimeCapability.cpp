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
: m_duration(2.0f * 3.141592654f)
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

/**
*  @brief
*    Get virtual time
*/
float VirtualTimeCapability::time() const
{
    return m_time;
}

/**
*  @brief
*    Set duration of a whole cycle (after that, time is reset to 0)
*/
void VirtualTimeCapability::setLoopDuration(float duration)
{
    assert(duration > 0.0f);

    m_duration = duration;

    normalizeTime();
}

/**
*  @brief
*    Update virtual time
*/
void VirtualTimeCapability::update(float delta)
{
    m_time += delta;

    setChanged(true);

    normalizeTime();
}

/**
*  @brief
*    Normalize time by wrapping it at the loop duration
*/
void VirtualTimeCapability::normalizeTime()
{
    while (m_time > m_duration)
    {
        m_time -= m_duration;

        setChanged(true);
    }
}


} // namespace gloperate
