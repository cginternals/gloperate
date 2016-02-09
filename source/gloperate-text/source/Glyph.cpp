
#include <gloperate-text/Glyph.h>


namespace gloperate_text
{


Glyph::Glyph()
: m_index(0u)
, m_advance(0)
{
}

Glyph::~Glyph()
{
}

GlyphIndex Glyph::index() const
{
    return m_index;
}

void Glyph::setIndex(const GlyphIndex index)
{
    m_index = index;
}

const glm::vec2 & Glyph::subTextureOrigin() const
{
    return m_subtextureOrigin;
}

void Glyph::setSubTextureOrigin(const glm::vec2 & origin)
{
    //assert(origin.x >= 0.f);
    //assert(origin.x <= 1.f);
    //assert(origin.y >= 0.f);
    //assert(origin.y <= 1.f);

    m_subtextureOrigin = origin;
}

const glm::vec2 & Glyph::subTextureExtent() const
{
    return m_subtextureExtent;
}

void Glyph::setSubTextureExtent(const glm::vec2 & extent)
{
    assert(extent.x <= 1.f);
    assert(extent.y <= 1.f);

    m_subtextureExtent = extent;
}

bool Glyph::depictable() const
{
    return m_subtextureExtent.x > 0.f && m_subtextureExtent.y > 0.f;
}

const glm::vec2 & Glyph::bearing() const
{
    return m_bearing;
}

void Glyph::setBearing(const glm::vec2 & bearing)
{
    m_bearing = bearing;
}

void Glyph::setBearing(const float fontAscent, const float xOffset, const float yOffset)
{
    m_bearing.x = xOffset;
    m_bearing.y = fontAscent - yOffset;
}

const glm::vec2 & Glyph::extent() const
{
    return m_extent;
}

void Glyph::setExtent(const glm::vec2 & extent)
{
    m_extent = extent;
}

float Glyph::advance() const
{
    return m_advance;
}

void Glyph::setAdvance(const float advance)
{
    m_advance = advance;
}

float Glyph::kerning(GlyphIndex subsequentIndex) const
{
    auto it = m_kernings.find(subsequentIndex);
    if (it == m_kernings.cend())
        return 0.f;

    return it->second;
}

void Glyph::setKerning(GlyphIndex subsequentIndex, const float kerning)
{
    m_kernings[subsequentIndex] = kerning;
}


} // namespace gloperate_text
