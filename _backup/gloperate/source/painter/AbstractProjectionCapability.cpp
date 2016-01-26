
#include <gloperate/painter/AbstractProjectionCapability.h>

#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate
{


AbstractProjectionCapability::AbstractProjectionCapability(AbstractViewportCapability * viewportCapability)
: AbstractCapability()
, m_viewportCapability(viewportCapability)
{
    viewportCapability->changed.connect([this](){this->onViewportChanged();});
}

AbstractProjectionCapability::~AbstractProjectionCapability()
{
}

void AbstractProjectionCapability::onViewportChanged()
{
    setAspectRatio(glm::ivec2(m_viewportCapability->width(), m_viewportCapability->height()));
}


} // namespace gloperate
