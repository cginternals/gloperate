
#include <gloperate-text/GlyphVertexCloud.h>

#include <gloperate/offsetof.h>


namespace gloperate_text
{


GlyphVertexCloud::GlyphVertexCloud()
{
}

GlyphVertexCloud::~GlyphVertexCloud()
{
}

const globjects::Texture * GlyphVertexCloud::texture() const
{
    return m_texture;
}

void GlyphVertexCloud::setTexture(globjects::Texture * texture)
{
    m_texture = texture;
}

gloperate::VertexDrawable * GlyphVertexCloud::drawable()
{
    return m_drawable;
}

const gloperate::VertexDrawable * GlyphVertexCloud::drawable() const
{
    return m_drawable;
}

GlyphVertexCloud::Vertices & GlyphVertexCloud::vertices()
{
    return m_vertices;
}

const GlyphVertexCloud::Vertices & GlyphVertexCloud::vertices() const
{
    return m_vertices;
}

gloperate::VertexDrawable * GlyphVertexCloud::createDrawable()
{
    auto drawable = new gloperate::VertexDrawable(Vertices(), gl::GL_POINTS);

    drawable->setFormats({
        gloperate::Format(3, gl::GL_FLOAT, GLOPERATE_OFFSETOF(Vertex, origin)),
        gloperate::Format(3, gl::GL_FLOAT, GLOPERATE_OFFSETOF(Vertex, vtan)),
        gloperate::Format(3, gl::GL_FLOAT, GLOPERATE_OFFSETOF(Vertex, vbitan)),
        gloperate::Format(4, gl::GL_FLOAT, GLOPERATE_OFFSETOF(Vertex, uvRect)),
    });

    drawable->bindAttributes({ 0, 1, 2, 3 });
    drawable->enableAll();

    return drawable;
}

void GlyphVertexCloud::update()
{
    if (!m_drawable)
        m_drawable = createDrawable();

    m_drawable->setVertices(m_vertices);
}


} // namespace gloperate_text
