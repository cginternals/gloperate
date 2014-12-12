/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>

#include <gloperate/painter/AbstractViewportCapability.h>

namespace gloperate {


/**
*  @brief
*    Constructor
*/
AbstractPerspectiveProjectionCapability::AbstractPerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractProjectionCapability(viewportCapability)
, m_viewportCapability(viewportCapability)
{
    viewportCapability->changed.connect([this](){this->onViewportChanged();});
}

/**
*  @brief
*    Destructor
*/
AbstractPerspectiveProjectionCapability::~AbstractPerspectiveProjectionCapability()
{
}

void AbstractPerspectiveProjectionCapability::onViewportChanged()
{
    setAspectRatio(glm::ivec2(m_viewportCapability->width(), m_viewportCapability->height()));
}

} // namespace gloperate
