#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <gloperate-text/gloperate-text_api.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{
    class Drawable;
}


namespace gloperate_text
{

class FontFace;
class GlyphSequence;


class GLOPERATE_TEXT_API GlyphVertexCloud
{
public:
    struct Vertex
    {
        glm::vec3 origin;
        glm::vec3 vtan;
        glm::vec3 vbitan;
        // vec2 lowerLeft and vec2 upperRight in glyph texture (uv)
        glm::vec4 uvRect;
        glm::vec4 fontColor;
    };

    using Vertices = std::vector<Vertex>;

public:
    GlyphVertexCloud();
    virtual ~GlyphVertexCloud();

    GlyphVertexCloud & operator=(const GlyphVertexCloud &) = delete;

    const globjects::Texture * texture() const;
    void setTexture(globjects::Texture * texture);

    gloperate::Drawable * drawable();
    const gloperate::Drawable * drawable() const;

    Vertices & vertices();
    const Vertices & vertices() const;

    void update();
    // allows for volatile optimizations
    void update(const Vertices & vertices);

    void optimize(
        const std::vector<GlyphSequence> & sequences
    ,   const FontFace & fontFace);

protected:
    static std::unique_ptr<gloperate::Drawable> createDrawable();

protected:
    Vertices m_vertices;

    std::unique_ptr<gloperate::Drawable> m_drawable;
    std::unique_ptr<globjects::Texture> m_texture;
};


} // namespace gloperate_text
