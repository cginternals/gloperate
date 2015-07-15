
#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>

#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate
{


AbstractPerspectiveProjectionCapability::AbstractPerspectiveProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractProjectionCapability(viewportCapability)
{
}

AbstractPerspectiveProjectionCapability::~AbstractPerspectiveProjectionCapability()
{
}

void AbstractPerspectiveProjectionCapability::onViewportChanged()
{
    setAspectRatio(glm::ivec2(m_viewportCapability->width(), m_viewportCapability->height()));
}


} // namespace gloperate
