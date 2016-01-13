
#include <gloperate-text/Glyph.h>


namespace gloperate_text
{


Glyph::Glyph()
: m_index(0)
, m_x(0)
, m_y(0)
, m_width(0)
, m_height(0)
, m_xOffset(0)
, m_yOffset(0)
, m_xAdvance(0)
{
}

Glyph::~Glyph()
{
}

std::uint32_t Glyph::index() const
{
    return m_index;
}

void Glyph::setIndex(std::uint32_t index)
{
    m_index = index;
}

std::uint32_t Glyph::x() const
{
    return m_x;
}

void Glyph::setX(std::uint32_t x)
{
    m_x = x;
}

std::uint32_t Glyph::y() const
{
    return m_y;
}

void Glyph::setY(std::uint32_t y)
{
    m_y = y;
}

std::uint32_t Glyph::width() const
{
    return m_width;
}

void Glyph::setWidth(std::uint32_t width)
{
    m_width = width;
}

std::uint32_t Glyph::height() const
{
    return m_height;
}

void Glyph::setHeight(std::uint32_t height)
{
    m_height = height;
}

std::uint32_t Glyph::xOffset() const
{
    return m_xOffset;
}

void Glyph::setXOffset(std::uint32_t xOffset)
{
    m_xOffset = xOffset;
}


} // namespace gloperate_text
