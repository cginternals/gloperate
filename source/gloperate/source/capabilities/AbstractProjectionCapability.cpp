/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/AbstractProjectionCapability.h>

#include <gloperate/capabilities/AbstractViewportCapability.h>

namespace gloperate {


/**
*  @brief
*    Constructor
*/
AbstractProjectionCapability::AbstractProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractCapability()
, m_viewportCapability(viewportCapability)
{
    viewportCapability->changed.connect([this](){this->onViewportChanged();});
}

/**
*  @brief
*    Destructor
*/
AbstractProjectionCapability::~AbstractProjectionCapability()
{
}

void AbstractProjectionCapability::onViewportChanged()
{
    setAspectRatio(glm::ivec2(m_viewportCapability->width(), m_viewportCapability->height()));
}

} // namespace gloperate
