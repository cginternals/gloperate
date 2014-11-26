/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/AbstractCameraCapability.h>

#include <gloperate/capabilities/AbstractViewportCapability.h>

namespace gloperate {


/**
*  @brief
*    Constructor
*/
AbstractCameraCapability::AbstractCameraCapability(AbstractViewportCapability * viewportCapability)
: AbstractCapability()
, m_viewportCapability(viewportCapability)
{
    viewportCapability->changed.connect([this](){this->onViewportChanged();});
}

/**
*  @brief
*    Destructor
*/
AbstractCameraCapability::~AbstractCameraCapability()
{
}


} // namespace gloperate
