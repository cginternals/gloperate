#include <gloperate/painter/ViewportCapability.h>


namespace gloperate
{

/**
*  @brief
*    Constructor
*/
ViewportCapability::ViewportCapability()
: m_x(0)
, m_y(0)
, m_width(0)
, m_height(0)
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
*    Get viewport position x
*/
int ViewportCapability::x() const
{
    return m_x;
}

/**
*  @brief
*    Get viewport position y
*/
int ViewportCapability::y() const
{
    return m_y;
}

/**
*  @brief
*    Get viewport width
*/
int ViewportCapability::width() const
{
    return m_width;
}

/**
*  @brief
*    Get viewport height
*/
int ViewportCapability::height() const
{
    return m_height;
}

/**
*  @brief
*    Set viewport
*/
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
