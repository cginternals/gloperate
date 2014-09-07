#include <gloperate/Viewport.h>


namespace gloperate
{


Viewport::Viewport()
: m_x(0)
, m_y(0)
, m_width(0)
, m_height(0)
{
}

Viewport::Viewport(int width, int height)
: m_x(0)
, m_y(0)
, m_width(width)
, m_height(height)
{
}

Viewport::Viewport(int x, int y, int width, int height)
: m_x(x)
, m_y(y)
, m_width(width)
, m_height(height)
{
}

Viewport::Viewport(const Viewport & viewport)
: m_x(viewport.m_x)
, m_y(viewport.m_y)
, m_width(viewport.m_width)
, m_height(viewport.m_height)
{
}

Viewport::~Viewport()
{
}

Viewport & Viewport::operator =(const Viewport & viewport)
{
    m_x      = viewport.m_x;
    m_y      = viewport.m_y;
    m_width  = viewport.m_width;
    m_height = viewport.m_height;
    return *this;
}

bool Viewport::operator ==(const Viewport & viewport) const
{
    return (m_x == viewport.m_x && m_y == viewport.m_y && m_width == viewport.m_width && m_height == viewport.m_height);
}

int Viewport::x() const
{
    return m_x;
}

void Viewport::setX(int x)
{
    m_x = x;
}

int Viewport::y() const
{
    return m_y;
}

void Viewport::setY(int y)
{
    m_y = y;
}

int Viewport::width() const
{
    return m_width;
}

void Viewport::setWidth(int width)
{
    m_width = width;
}

int Viewport::height() const
{
    return m_height;
}

void Viewport::setHeight(int height)
{
    m_height = height;
}

void Viewport::setViewport(int x, int y, int width, int height)
{
    m_x      = x;
    m_y      = y;
    m_width  = width;
    m_height = height;
}


} // gloperate
