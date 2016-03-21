
#include <gloperate-text/Typesetter.h>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>


namespace gloperate_text
{

const char32_t & Typesetter::lineFeed()
{
    static const auto LF = static_cast<char32_t>('\x0A');
    return LF;
}

glm::vec2 Typesetter::extent(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const float fontSize)
{
    return typeset(sequence, fontFace
        , GlyphVertexCloud::Vertices::iterator(), true) * fontSize / fontFace.size();
}

glm::vec2 Typesetter::typeset(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const GlyphVertexCloud::Vertices::iterator & begin
,   bool dryrun)
{
    //const auto & padding = fontFace.glyphTexturePadding();

    auto pen = glm::vec2(0.f);
    auto vertex = begin;
    auto extent = glm::vec2(0.f);

    const auto iBegin = sequence.string().cbegin();
    const auto iEnd = sequence.string().cend();

    // iterator used to reduce the number of wordwrap forward passes
    auto safe_forward = iBegin;

    auto feedLine = false;
    auto feedVertex = vertex;

    for (auto i = iBegin; i != iEnd; ++i)
    {
        const auto & glyph = fontFace.glyph(*i);

        // handle line feeds as well as word wrap for next word (or 
        // next glyph if word width exceeds the max line width)
        feedLine = *i == lineFeed() || (sequence.wordWrap() &&
            typeset_wordwrap(sequence, fontFace, pen, glyph, i, safe_forward));

        if (feedLine)
        {
            assert(i != iBegin);
            typeset_extent(fontFace, i - 1, iBegin, pen, extent);

            // handle alignment (when line feed occurs)
            if (!dryrun)
                typeset_align(pen, sequence.alignment(), feedVertex, vertex);

            pen.x = 0.f;
            pen.y -= fontFace.lineHeight();

            feedLine = false;
            feedVertex = vertex;
        }
        else if (i != iBegin) // apply kerning
            pen.x += fontFace.kerning(*(i - 1), *i);

        // typeset glyphs in vertex cloud (only if renderable)
        if (!dryrun && glyph.depictable())
            typeset_glyph(fontFace, pen, glyph, vertex++);

        pen.x += glyph.advance();

        if (i + 1 == iEnd) // handle alignment (when last line of sequence is processed)
        {
            typeset_extent(fontFace, i, iBegin, pen, extent);

            if (!dryrun)
                typeset_align(pen, sequence.alignment(), feedVertex, vertex);
        }
    }

    if (!dryrun)
    {
        anchor_transform(sequence, fontFace, begin, vertex);
        vertex_transform(sequence.transform(), begin, vertex);
    }

    return extent_transform(sequence, extent);
}

inline bool Typesetter::typeset_wordwrap(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const glm::vec2 & pen
,   const Glyph & glyph
,   const std::u32string::const_iterator & index
,   std::u32string::const_iterator & safe_forward)
{
    assert(sequence.wordWrap());

    const auto lineWidth = sequence.lineWidth();
    auto width_forward = 0.f;

    const auto pen_glyph = pen.x + glyph.advance()
        + (index != sequence.string().cbegin() ? fontFace.kerning(*(index - 1), *index) : 0.f);

    const auto wrap_glyph = glyph.depictable() && pen_glyph > lineWidth
        && (glyph.advance() <= lineWidth || pen.x > 0.f);

    auto wrap_forward = false;
    if (!wrap_glyph && index >= safe_forward)
    {
        safe_forward = typeset_forward(sequence, fontFace, index, width_forward);
        wrap_forward = width_forward <= lineWidth && (pen.x + width_forward) > lineWidth;
    }

    return wrap_forward || wrap_glyph;
}

inline std::u32string::const_iterator Typesetter::typeset_forward(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const std::u32string::const_iterator & begin
,   float & width)
{
    // setup common delimiters 
    // Note: u32string::find outperforms set::count here (tested)

	static const auto delimiters = std::u32string({ '\x0A', ' ', ',', '.', '-', '/', '(', ')', '[', ']', '<', '>' });

    const auto iBegin = sequence.string().cbegin();
    const auto iEnd = sequence.string().cend();

    width = 0.f; // reset the width 

    // accumulate glyph advances (including kerning) up to the next
    // delimiter occurence starting at begin of the string.
    auto i = begin;
    while (i != iEnd && delimiters.find(*i) == delimiters.npos)
    {
        if (i != iBegin)
            width += fontFace.kerning(*(i - 1), *i);

        width += fontFace.glyph(*i++).advance();
    }
    return i;
}

inline void Typesetter::typeset_glyph(
    const FontFace & fontFace
,   const glm::vec2 & pen
,   const Glyph & glyph
,   const GlyphVertexCloud::Vertices::iterator & vertex)
{
    const auto & padding = fontFace.glyphTexturePadding();
    vertex->origin    = glm::vec3(pen, 0.f);
    vertex->origin.x += glyph.bearing().x - padding[3];
    vertex->origin.y += glyph.bearing().y - glyph.extent().y - padding[2];

    vertex->vtan   = glm::vec3(glyph.extent().x + padding[1] + padding[3], 0.f, 0.f);
    vertex->vbitan = glm::vec3(0.f, glyph.extent().y + padding[0] + padding[2], 0.f);

    const auto extentScale = 1.f / glm::vec2(fontFace.glyphTextureExtent());
    const auto ll = glyph.subTextureOrigin() 
        - glm::vec2(padding[3], padding[2]) * extentScale;
    const auto ur = glyph.subTextureOrigin() + glyph.subTextureExtent() 
        + glm::vec2(padding[1], padding[0]) * extentScale;
    vertex->uvRect = glm::vec4(ll, ur);
}

inline void Typesetter::typeset_extent(
    const FontFace & fontFace
,   std::u32string::const_iterator index
,   const std::u32string::const_iterator & begin
,   glm::vec2 & pen
,   glm::vec2 & extent)
{
    // on line feed, revert advance of preceding, not depictable glyphs
    while (index > begin)
    {
        auto precedingGlyph = fontFace.glyph(*index);
        if (precedingGlyph.depictable())
            break;

        pen.x -= precedingGlyph.advance();
        --index;
    }
    extent.x = glm::max(pen.x, extent.x);
    extent.y += fontFace.lineHeight();
}

inline void Typesetter::typeset_align(
    const glm::vec2 & pen
,   const Alignment alignment
,   const GlyphVertexCloud::Vertices::iterator & begin
,   const GlyphVertexCloud::Vertices::iterator & end)
{
    if (alignment == Alignment::LeftAligned)
        return;

    auto penOffset = -pen.x;

    if (alignment == Alignment::Centered)
        penOffset *= 0.5f;

    // origin is expected to be in 'font face space' (not transformed)
    for (auto v = begin; v != end; ++v)
        v->origin.x += penOffset;
}

inline void Typesetter::anchor_transform(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const GlyphVertexCloud::Vertices::iterator & begin
,   const GlyphVertexCloud::Vertices::iterator & end)
{
    auto offset = 0.f;

    switch (sequence.lineAnchor())
    {
    case LineAnchor::Ascent:
        offset = fontFace.ascent();
        break;
    case LineAnchor::Center:
        offset = fontFace.size() * 0.5f + fontFace.descent();
        break;
    case LineAnchor::Descent:
        offset = fontFace.descent();
        break;
    case LineAnchor::Baseline:
    default:
        return;
    }

    for (auto v = begin; v != end; ++v)
        v->origin.y -= offset;
}

inline void Typesetter::vertex_transform(
    const glm::mat4 & transform
,   const GlyphVertexCloud::Vertices::iterator & begin
,   const GlyphVertexCloud::Vertices::iterator & end)
{
    for (auto v = begin; v != end; ++v)
    {
        auto ll = transform * glm::vec4(v->origin, 1.f);
        auto lr = transform * glm::vec4(v->origin + v->vtan, 1.f);
        auto ul = transform * glm::vec4(v->origin + v->vbitan, 1.f);

        v->origin = glm::vec3(ll);
        v->vtan   = glm::vec3(lr - ll);
        v->vbitan = glm::vec3(ul - ll);
    }
}

inline glm::vec2 Typesetter::extent_transform(
    const GlyphSequence & sequence
,   const glm::vec2 & extent)
{
    auto ll = sequence.transform() * glm::vec4(     0.f,      0.f, 0.f, 1.f);
    auto lr = sequence.transform() * glm::vec4(extent.x,      0.f, 0.f, 1.f);
    auto ul = sequence.transform() * glm::vec4(     0.f, extent.y, 0.f, 1.f);

    return glm::vec2(glm::distance(lr, ll), glm::distance(ul, ll));
}



} // namespace gloperate_text
