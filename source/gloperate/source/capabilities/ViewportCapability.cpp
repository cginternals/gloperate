#include <gloperate/capabilities/ViewportCapability.h>

namespace gloperate
{

ViewportCapability::ViewportCapability()
{
}

ViewportCapability::~ViewportCapability()
{
}

void ViewportCapability::setViewport(const Viewport & viewport)
{
    m_viewport = viewport;
}

const Viewport & ViewportCapability::viewport() const
{
    return m_viewport;
}

int ViewportCapability::x() const
{
    return m_viewport.x();
}

int ViewportCapability::y() const
{
    return m_viewport.y();
}

int ViewportCapability::width() const
{
    return m_viewport.width();
}

int ViewportCapability::height() const
{
    return m_viewport.height();
}

} // namespace gloperate
