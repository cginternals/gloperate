
#include <gloperate-text/FontFace.h>


namespace gloperate_text
{


FontFace::FontFace()
: m_ascent (0.f)
, m_descent(0.f)
, m_linegap(0.f)
{
}

FontFace::~FontFace()
{
}

float FontFace::size() const
{
    // Note: m_descent is usually negative.
    return m_ascent - m_descent;
}

float FontFace::ascent() const
{
    return m_ascent;
}

void FontFace::setAscent(const float ascent)
{
    assert(ascent > 0.f);
    m_ascent = ascent;
}

float FontFace::descent() const
{
    return m_descent;
}

void FontFace::setDescent(const float descent)
{
    // Note: No assert here: 
    // There might be fonts with their lowest descender above baseline.
    // assert(descent < 0.f); 

    m_descent = descent;
}

float FontFace::linegap() const
{
    return m_linegap;
}

void FontFace::setLinegap(const float linegap)
{
    m_linegap = linegap;
}

float FontFace::linespace() const
{
    if (lineHeight() == 0.f)
        return lineHeight();

    return size() / lineHeight();
}

void FontFace::setLinespace(const float spacing)
{
    m_linegap = size() * (spacing - 1);
}

float FontFace::lineHeight() const
{
    return size() + linegap();
}

void FontFace::setLineHeight(const float lineHeight)
{
    m_linegap = lineHeight - size();
}

const glm::uvec2 & FontFace::glyphTextureExtent() const
{
    return m_glyphTextureExtent;
}

void FontFace::setGlyphTextureExtent(const glm::uvec2 & extent)
{
    assert(extent.x > 0);
    assert(extent.y > 0);

    m_glyphTextureExtent = extent;
}

const glm::vec4 & FontFace::glyphTexturePadding() const
{
    return m_glyphTexturePadding;
}

void FontFace::setGlyphTexturePadding(const glm::vec4 & padding)
{
    assert(padding[0] >= 0.f);
    assert(padding[1] >= 0.f);
    assert(padding[2] >= 0.f);
    assert(padding[3] >= 0.f);

    m_glyphTexturePadding = padding;
}

globjects::Texture * FontFace::glyphTexture() const
{
    return m_glyphTexture;
}

void FontFace::setGlyphTexture(globjects::Texture * texture)
{
    m_glyphTexture = texture;
}

bool FontFace::hasGlyph(const GlyphIndex index) const
{
    return m_glyphs.find(index) != m_glyphs.cend();
}

Glyph & FontFace::glyph(const GlyphIndex index)
{
    const auto existing = m_glyphs.find(index);
    if (existing != m_glyphs.cend())
        return existing->second;

    auto glyph = Glyph();
    glyph.setIndex(index);

    const auto inserted = m_glyphs.emplace(glyph.index(), glyph);
    return inserted.first->second;
}

const Glyph & FontFace::glyph(const GlyphIndex index) const
{
    const auto existing = m_glyphs.find(index);
    if (existing != m_glyphs.cend())
        return existing->second;

    static const auto empty = Glyph();
    return empty;
}

void FontFace::addGlyph(const Glyph & glyph)
{
    assert(m_glyphs.find(glyph.index()) == m_glyphs.cend());

    m_glyphs.emplace(glyph.index(), glyph);
}

std::vector<GlyphIndex> FontFace::glyphs() const
{
    auto glyphs = std::vector<GlyphIndex>();
    for (const auto & i : m_glyphs)
        glyphs.push_back(i.first);

    return glyphs;
}

bool FontFace::depictable(const GlyphIndex index) const
{
    return glyph(index).depictable();
}

float FontFace::kerning(const GlyphIndex index, const GlyphIndex subsequentIndex) const
{
    const auto it = m_glyphs.find(index);
    if (it == m_glyphs.cend())
        return 0.f;

    return it->second.kerning(subsequentIndex);
}

void FontFace::setKerning(const GlyphIndex index, const GlyphIndex subsequentIndex, const float kerning)
{
    const auto it = m_glyphs.find(index);
    if (it == m_glyphs.cend() || !hasGlyph(subsequentIndex))
    {
        assert(false);
        return;
    }

    it->second.setKerning(subsequentIndex, kerning);
}


} // namespace gloperate_text
