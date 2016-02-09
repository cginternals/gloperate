
#pragma once

#include <glm/fwd.hpp>

#include <gloperate-text/GlyphVertexCloud.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{
enum class HorizontalAlignment : unsigned char;

class GlyphSequence;
class FontFace;
class Glyph;


class GLOPERATE_TEXT_API Typesetter
{
public:
    Typesetter() = delete;
    virtual ~Typesetter() = delete;

    static const char32_t & lineFeed();

    static glm::vec2 extent(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   float fontSize);

    static glm::vec2 typeset(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const GlyphVertexCloud::Vertices::iterator & begin
    ,   bool dryrun = false);

private:

    inline static bool typeset_wordwrap(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const glm::vec2 & pen
    ,   const Glyph & glyph
    ,   const std::u32string::const_iterator & index
    ,   std::u32string::const_iterator & safe_forward);

    inline static std::u32string::const_iterator typeset_forward(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const std::u32string::const_iterator & begin
    ,   float & width);

    inline static void typeset_glyph(
        const glm::vec2 & pen
    ,   const Glyph & glyph
    ,   const GlyphVertexCloud::Vertices::iterator & vertex);

    inline static void typeset_extent(
        const FontFace & fontFace
    ,   const std::u32string::const_iterator & preceding
    ,   const glm::vec2 & pen
    ,   glm::vec2 & extent);

    inline static void typeset_align(
        const glm::vec2 & pen
    ,   const HorizontalAlignment alignment
    ,   const GlyphVertexCloud::Vertices::iterator & begin
    ,   const GlyphVertexCloud::Vertices::iterator & end);

    inline static void typeset_transform(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const GlyphVertexCloud::Vertices::iterator & begin);
};


} // namespace gloperate_text
