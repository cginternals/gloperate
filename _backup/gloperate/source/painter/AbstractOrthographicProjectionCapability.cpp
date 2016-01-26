
#include <gloperate/painter/AbstractOrthographicProjectionCapability.h>

#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate
{


AbstractOrthographicProjectionCapability::AbstractOrthographicProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractProjectionCapability(viewportCapability)
{
}

AbstractOrthographicProjectionCapability::~AbstractOrthographicProjectionCapability()
{
}

void AbstractOrthographicProjectionCapability::onViewportChanged()
{
    setAspectRatio(glm::ivec2(m_viewportCapability->width(), m_viewportCapability->height()));
}


} // namespace gloperate
