/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {


/**
*  @brief
*    Constructor
*/
AbstractCapability::AbstractCapability()
: m_changed(true)
{
}

/**
*  @brief
*    Destructor
*/
AbstractCapability::~AbstractCapability()
{
}

/**
 * @brief Returns whether the information of this Capability have changed.
 * @return the changed flag
 */
bool AbstractCapability::hasChanged() const
{
    return m_changed;
}

/**
 * @brief AbstractCapability::setChanged
 * @param changed the new value of the changed flag
 */
void AbstractCapability::setChanged(bool _changed)
{
    m_changed = _changed;
}


} // namespace gloperate
