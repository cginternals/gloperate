
#include <gloperate/painter/ViewportCapability.h>


namespace gloperate
{


ViewportCapability::ViewportCapability()
: m_x(0)
, m_y(0)
, m_width(0)
, m_height(0)
{
}

ViewportCapability::~ViewportCapability()
{
}

int ViewportCapability::x() const
{
    return m_x;
}

int ViewportCapability::y() const
{
    return m_y;
}

int ViewportCapability::width() const
{
    return m_width;
}

int ViewportCapability::height() const
{
    return m_height;
}

void ViewportCapability::setViewport(int x, int y, int width, int height)
{
    // Save new viewport
    m_x      = x;
    m_y      = y;
    m_width  = width;
    m_height = height;

    // Mark viewport as changed
    setChanged(true);
}


} // namespace gloperate
