
#include <gloperate-text/GlyphSequence.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gloperate-text/FontFace.h>


namespace gloperate_text
{


GlyphSequence::GlyphSequence()
: m_wordWrap(false)
, m_lineWidth(0.f)
, m_alignment(Alignment::LeftAligned)
, m_anchor(LineAnchor::Baseline)
{
}

GlyphSequence::~GlyphSequence()
{
}

size_t GlyphSequence::size() const
{
    return m_string.size();
}

size_t GlyphSequence::size(const FontFace & fontFace) const
{
    auto count = size_t(0);
    for (const auto & c : m_string)
    {
        if (fontFace.depictable(c))
            ++count;
    }
    return count;
}

const std::u32string & GlyphSequence::string() const
{
    return m_string;
}

void GlyphSequence::setString(const std::u32string & string)
{
    if (m_string.compare(string) == 0)
        return;

    m_string = string;
}

const std::vector<char32_t> & GlyphSequence::chars(
    std::vector<char32_t> & allChars) const
{
    allChars.reserve(allChars.size() + size());

    for (const auto & c : m_string)
        allChars.push_back(c);

    return allChars;
}

const std::vector<char32_t> & GlyphSequence::chars(
    std::vector<char32_t> & depictableChars
,   const FontFace & fontFace) const
{
    depictableChars.reserve(depictableChars.size() + size(fontFace));

    for (const auto & c : m_string)
    {
        if(fontFace.depictable(c))
            depictableChars.push_back(c);
    }
    return depictableChars;
}

bool GlyphSequence::wordWrap() const
{
    return m_wordWrap;
}

void GlyphSequence::setWordWrap(bool enable)
{
    m_wordWrap = enable;
}

float GlyphSequence::lineWidth() const
{
    return m_lineWidth;
}

void GlyphSequence::setLineWidth(
    float lineWidth
,   float fontSize
,   const FontFace & fontFace)
{
    // since typesetting is done in the font faces font size, the 
    // given linewidth has to be scaled to the font faces font size
    m_lineWidth = glm::max(lineWidth * fontFace.size() / fontSize, 0.f);
}

Alignment GlyphSequence::alignment() const
{
    return m_alignment;
}

void GlyphSequence::setAlignment(const Alignment alignment)
{
    m_alignment = alignment;
}

LineAnchor GlyphSequence::lineAnchor() const
{
    return m_anchor;
}

void GlyphSequence::setLineAnchor(const LineAnchor anchor)
{
    m_anchor = anchor;
}

const char32_t & GlyphSequence::lineFeed()
{
    static const auto LF = static_cast<char32_t>('\x0A');
    return LF;
}

const glm::mat4 & GlyphSequence::transform() const
{
    return m_transform;
}

void GlyphSequence::setTransform(const glm::mat4 & transform)
{
    m_transform = transform;
}

void GlyphSequence::setTransform(
    const glm::vec2 & origin
,   const float fontSize
,   const FontFace & fontFace
,   const glm::uvec2 & viewportExtent)
{   
    m_transform = glm::mat4();

    // translate to lower left in NDC
    m_transform = glm::translate(m_transform, glm::vec3(-1.f, -1.f, 0.f));
    // translate to origin in screen space
    m_transform = glm::translate(m_transform, glm::vec3(origin, 0.f));
    // scale glyphs of font face to target normalized size
    m_transform = glm::scale(m_transform, glm::vec3(viewportExtent.y * fontSize / fontFace.size()));
    // scale glyphs to NDC size
    m_transform = glm::scale(m_transform, 2.f / glm::vec3(viewportExtent.x, viewportExtent.y, 1.f));
}

void GlyphSequence::setTransform(
    const glm::vec2 & origin
,   const float fontSize
,   const FontFace & fontFace
,   const glm::uvec2 & viewportExtent
,   const float pixelPerInch
,   const glm::vec4 & margins)
{
    m_transform = glm::mat4();

    const auto pointsPerInch = 72.f;
    const auto ppiScale = pixelPerInch / pointsPerInch;

    // translate to lower left in NDC
    m_transform = glm::translate(m_transform, glm::vec3(-1.f, -1.f, 0.f));
    // scale glyphs to NDC size
    m_transform = glm::scale(m_transform, 2.f / glm::vec3(viewportExtent.x, viewportExtent.y, 1.f));
    // scale glyphs to pixel size with respect to the displays ppi
    m_transform = glm::scale(m_transform, glm::vec3(ppiScale));
    // translate to origin in point space - scale origin within 
    // margined extend (i.e., viewport with margined areas removed)
    const auto marginedExtent = glm::vec2(viewportExtent.x, viewportExtent.y) / ppiScale
        - glm::vec2(margins[3] + margins[1], margins[2] + margins[0]);
    m_transform = glm::translate(m_transform
        , glm::vec3((0.5f * origin + 0.5f) * marginedExtent, 0.f) + glm::vec3(margins[3], margins[2], 0.f));
    // scale glyphs of font face to target font size
    m_transform = glm::scale(m_transform, glm::vec3(fontSize / fontFace.size()));
}


} // namespace gloperate_text
