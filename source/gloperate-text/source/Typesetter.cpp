
#include <gloperate-text/Typesetter.h>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>


namespace gloperate_text
{

const char32_t & Typesetter::lineFeed()
{
    static const auto LF = static_cast<char32_t>('\u000A');
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
            typeset_extent(fontFace, i - 1, pen, extent);

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
            typeset_glyph(pen, glyph, vertex++);

        pen.x += glyph.advance();

        if (i + 1 == iEnd) // handle alignment (when last line of sequence is processed)
        {
            typeset_extent(fontFace, i - 1, pen, extent);

            if (!dryrun)
                typeset_align(pen, sequence.alignment(), feedVertex, vertex);
        }
    }

    if (!dryrun) // transform glyphs in vertex cloud
        typeset_transform(sequence, fontFace, begin);

    // ToDo: refine to support 3D transforms
    auto ll = glm::vec4(0.f, 0.f, 0.f, 1.f);
    auto ur = ll + glm::vec4(extent, 0.f, 0.f);

    ll = sequence.transform() * ll;
    ur = sequence.transform() * ur;

    return glm::vec2(ur.x - ll.x, ur.y - ll.y);
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

    auto width_forward = 0.f;

    if (index >= safe_forward)
        safe_forward = typeset_forward(sequence, fontFace, index, width_forward);

    const auto pen_forward = pen.x + width_forward;
    const auto pen_glyph = pen.x + glyph.advance()
        + (index != sequence.string().cbegin() ? fontFace.kerning(*(index - 1), *index) : 0.f);

    const auto lineWidth = sequence.lineWidth();
    const auto wrap_forward = width_forward <= lineWidth && pen_forward > lineWidth;
    const auto wrap_glyph = pen_glyph > lineWidth 
        && (glyph.advance() <= lineWidth || pen.x > 0.f);

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
    static const auto delimiters = std::u32string(U"\u000A ,.-/()[]<>");

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
    while (i != iEnd && delimiters.find(*i) != delimiters.npos)
    {
        if (i != iBegin)
            width += fontFace.kerning(*(i - 1), *i);

        width += fontFace.glyph(*i++).advance();
    }
    return i;
}

inline void Typesetter::typeset_glyph(
    const glm::vec2 & pen
,   const Glyph & glyph
,   const GlyphVertexCloud::Vertices::iterator & vertex)
{
    vertex->origin = pen + glm::vec2(
        glyph.bearing().x, glyph.bearing().y - glyph.extent().y);

    vertex->extent = glyph.extent();

    vertex->uvRect = glm::vec4(
        glyph.subTextureOrigin(), glyph.subTextureOrigin() + glyph.subTextureExtent());
}

inline void Typesetter::typeset_extent(
    const FontFace & fontFace
,   const std::u32string::const_iterator & preceding
,   const glm::vec2 & pen
,   glm::vec2 & extent)
{
    auto lineWidth = pen.x;

    // on line feed, if last/preceding glyph is not depictable
    // revert its advance (important for horizontal alignment).
    auto precedingGlyph = fontFace.glyph(*(preceding));
    if (!precedingGlyph.depictable())
        lineWidth -= precedingGlyph.advance();

    extent.x = glm::max(lineWidth, extent.x);
    extent.y += fontFace.lineHeight();
}

inline void Typesetter::typeset_align(
    const glm::vec2 & pen
,   const HorizontalAlignment alignment
,   const GlyphVertexCloud::Vertices::iterator & begin
,   const GlyphVertexCloud::Vertices::iterator & end)
{
    if (alignment == HorizontalAlignment::Left)
        return;

    auto penOffset = -pen.x;

    if (alignment == HorizontalAlignment::Center)
        penOffset *= 0.5f;

    // origin is expected to be in 'font face space' (not transformed)
    for (auto v = begin; v != end; ++v)
        v->origin.x += penOffset;
}

inline void Typesetter::typeset_transform(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const GlyphVertexCloud::Vertices::iterator & begin)
{
    auto vertex = begin;

    const auto iEnd = sequence.string().cend();
    for (auto i = sequence.string().cbegin(); i != iEnd; ++i)
    {
        if (!fontFace.depictable(*i))
            continue;

        // ToDo: refine to support 3D transforms

        auto ll = glm::vec4(vertex->origin, 0.f, 1.f);
        auto ur = ll + glm::vec4(vertex->extent, 0.f, 0.f);

        ll = sequence.transform() * ll;
        ur = sequence.transform() * ur;

        vertex->origin.x = ll.x;
        vertex->origin.y = ll.y;
        // transform back to extent
        vertex->extent.x = ur.x - ll.x;
        vertex->extent.y = ur.y - ll.y;

        ++vertex;
    }
}


} // namespace gloperate_text
