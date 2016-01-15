
#include <gloperate-text/TextLayouter.h>

#include <stringzeug/conversion.h>

#include <glm/gtx/random.hpp>

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

void TextLayouter::layout(Text & text, FontFace * font, GlyphVertexCloud & vertexCloud)
{
    const std::u32string & characters = text.characters();

    //vertexCloud.vertices.resize(32 * 32 * characters.size());
    vertexCloud.vertices.resize(characters.size());

    float width = 2.0 / characters.size();

    std::uint32_t glyphTextureWidth = stringzeug::fromString<std::uint32_t>(font->configuration("scaleW", "0"));
    //std::uint32_t glyphTextureHeight = stringzeug::fromString<std::uint32_t>(font->configuration("scaleH", "0"));

    //for (size_t y = 0; y < 32; ++y)
    //{
        //for (size_t x = 0; x < 32; ++x)
        //{
            //glm::vec3 start = glm::vec3(x / 32.0 * 2.0 - 1.0, y / 32.0 * 2.0 - 1.0, 0);
            //glm::vec3 end = glm::vec3((x+1) / 32.0 * 2.0 - 1.0, (y+1) / 32.0 * 2.0 - 1.0, 0);

            for (size_t i = 0; i < characters.size(); ++i)
            {
                //Glyph & glyph = font->glyph(static_cast<int>(glm::linearRand(65.0, 80.0)));
                Glyph & glyph = font->glyph(characters[i]);
                //GlyphVertex & vertex = vertexCloud.vertices[i * 32 * 32 + x * 32 + y];
                GlyphVertex & vertex = vertexCloud.vertices[i];

                vertex.coord = glm::vec3(i * width - 1.0, -0.5, 0.0);
                vertex.tangent = glm::vec3(width, 0.0, 0.0);
                vertex.bitangent = glm::vec3(0.0, 1.0, 0.0);
                vertex.glyphStart = glm::vec2(glyph.x(), 0.0) / glm::vec2(glyphTextureWidth, 1.0);
                vertex.glyphEnd = glm::vec2(glyph.x() + glyph.width(), 1.0) / glm::vec2(glyphTextureWidth, 1.0);

                //vertex.coord = start + ((vertex.coord + glm::vec3(1.0, 1.0, 0.0)) / glm::vec3(2.0)) * (end - start);
                //vertex.tangent = glm::vec3(vertex.tangent.x / 32.0, vertex.tangent.y / 32.0, vertex.tangent.z);
                //vertex.bitangent = glm::vec3(vertex.bitangent.x / 32.0, vertex.bitangent.y / 32.0, vertex.bitangent.z);
            }
        //}
    //}

    vertexCloud.glyphTexture = font->glyphTexture();

    vertexCloud.verticesChanged();
}

void TextLayouter::layout(TextManager & manager, FontFace * font, GlyphVertexCloud & vertexCloud)
{
    assert(false);
}


} // namespace gloperate_text
