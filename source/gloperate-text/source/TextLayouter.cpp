
#include <gloperate-text/TextLayouter.h>

#include <stringzeug/conversion.h>

#include <glm/vec3.hpp>

#include <gloperate-text/geometry/GlyphVertexCloud.h>
#include <gloperate-text/geometry/GlyphVertex.h>
#include <gloperate-text/FontFace.h>
#include <gloperate-text/Text.h>


namespace gloperate_text
{


TextLayouter::TextLayouter()
{
}

TextLayouter::~TextLayouter()
{
}

void TextLayouter::layout(const Text & text, FontFace * font, GlyphVertexCloud & vertexCloud)
{
    vertexCloud.vertices.resize(text.characters().size());

    vertexCloud.glyphTexture = font->glyphTexture();

    basicLayout(text, font, &vertexCloud.vertices[0], glm::vec2(-1.0, -1.0), glm::vec2(1.0, 1.0));

    vertexCloud.verticesChanged();
}

void TextLayouter::layout(const TextManager & manager, FontFace * font, GlyphVertexCloud & vertexCloud)
{
    assert(false);
}

void TextLayouter::basicLayout(const Text & text, FontFace * font, GlyphVertex * startVertex, const glm::vec2 & start, const glm::vec2 & end)
{
    std::uint32_t glyphTextureWidth = stringzeug::fromString<std::uint32_t>(font->configuration("scaleW", "0"));
    //std::uint32_t glyphTextureHeight = stringzeug::fromString<std::uint32_t>(font->configuration("scaleH", "0"));

    float width = 1.0 / text.characters().size();

    for (size_t i = 0; i < text.characters().size(); ++i)
    {
        Glyph & glyph = font->glyph(text.characters()[i]);
        GlyphVertex & vertex = *(startVertex + i);

        vertex.worldStart = start + glm::vec2(i * width, 0.0) * (end - start);
        vertex.worldEnd = vertex.worldStart + glm::vec2(width * (end.x - start.x), end.y - start.y);
        vertex.glyphStart = glm::vec2(glyph.x(), 0.0) / glm::vec2(glyphTextureWidth, 1.0);
        vertex.glyphEnd = glm::vec2(glyph.x() + glyph.width(), 1.0) / glm::vec2(glyphTextureWidth, 1.0);
    }
}


} // namespace gloperate_text
