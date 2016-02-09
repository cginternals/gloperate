
#pragma once

#include <string>

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate-text/Alignment.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{

class FontFace;
class Glyph;


class GLOPERATE_TEXT_API GlyphSequence
{
public:
    GlyphSequence();
    virtual ~GlyphSequence();

    static const char32_t & lineFeed();

    size_t size() const;
    size_t size(const FontFace & fontFace) const;

    const std::u32string & string() const;
    void setString(const std::u32string & string);

    bool wordWrap() const;
    void setWordWrap(bool enable);

    float lineWidth() const;
    void setLineWidth(
        float lineWidth
    ,   float fontSize
    ,   const FontFace & fontFace);

    HorizontalAlignment alignment() const;
    void setAlignment(HorizontalAlignment alginment);

    const glm::mat4 & transform() const;
    void setTransform(const glm::mat4 & transform);

    void setTransform(
        const glm::vec2 & origin
    ,   float fontSize
    ,   const FontFace & fontFace
    ,   const glm::uvec2 & viewportExtent);

    void setTransform(
        const glm::vec2 & origin
    ,   float fontSize
    ,   const FontFace & fontFace
    ,   const glm::uvec2 & viewportExtent
    ,   float pixelPerInch
    ,   const glm::vec4 & margins = glm::vec4(0.f, 0.f, 0.f, 0.f));

protected:
    std::u32string m_string;

    bool m_wordWrap;
    float m_lineWidth;

    HorizontalAlignment m_alignment;

    glm::mat4 m_transform;
};


} // namespace gloperate_text
