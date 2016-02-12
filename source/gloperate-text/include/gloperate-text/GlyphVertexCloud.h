#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/primitives/VertexDrawable.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


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
    };

    using Vertices = std::vector<Vertex>;

public:
    GlyphVertexCloud();
    virtual ~GlyphVertexCloud();

    const globjects::Texture * texture() const;
    void setTexture(globjects::Texture * texture);

    gloperate::VertexDrawable * drawable();
    const gloperate::VertexDrawable * drawable() const;

    Vertices & vertices();
    const Vertices & vertices() const;

    void update();

protected:
    static gloperate::VertexDrawable * createDrawable();

protected:
    Vertices m_vertices;

    globjects::ref_ptr<gloperate::VertexDrawable> m_drawable;
    globjects::ref_ptr<globjects::Texture> m_texture;
};


} // namespace gloperate_text