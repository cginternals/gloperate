/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/ViewportCapability.h>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
ViewportCapability::ViewportCapability()
{
}

/**
*  @brief
*    Destructor
*/
ViewportCapability::~ViewportCapability()
{
}

/**
*  @brief
*    Get viewport
*/
const Viewport & ViewportCapability::viewport() const
{
    return m_viewport;
}

/**
*  @brief
*    Set viewport
*/
void ViewportCapability::setViewport(const Viewport & viewport)
{
    m_viewport = viewport;

    setChanged(true);
}


} // namespace gloperate
