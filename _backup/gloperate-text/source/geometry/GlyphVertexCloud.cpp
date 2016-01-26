
#include <gloperate-text/geometry/GlyphVertexCloud.h>

#include <gloperate/offsetof.h>

#include <gloperate/primitives/VertexDrawable.h>


namespace gloperate_text
{


GlyphVertexCloud::GlyphVertexCloud()
{
}

void GlyphVertexCloud::verticesChanged()
{
    if (!m_drawable)
    {
        m_drawable = new gloperate::VertexDrawable(std::vector<GlyphVertex>(), gl::GL_POINTS);

        m_drawable->setFormats({
            gloperate::Format(2, gl::GL_FLOAT, GLOPERATE_OFFSETOF(GlyphVertex, worldStart)),
            gloperate::Format(2, gl::GL_FLOAT, GLOPERATE_OFFSETOF(GlyphVertex, worldEnd)),
            gloperate::Format(2, gl::GL_FLOAT, GLOPERATE_OFFSETOF(GlyphVertex, glyphStart)),
            gloperate::Format(2, gl::GL_FLOAT, GLOPERATE_OFFSETOF(GlyphVertex, glyphEnd))
        });

        m_drawable->bindAttributes({ 0, 1, 2, 3 });

        m_drawable->enableAll();
    }

    m_drawable->setVertices(vertices);
}

gloperate::VertexDrawable * GlyphVertexCloud::drawable()
{
    return m_drawable;
}

const gloperate::VertexDrawable * GlyphVertexCloud::drawable() const
{
    return m_drawable;
}


} // namespace gloperate_text
