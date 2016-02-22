
#include <gloperate-text/GlyphVertexCloud.h>

#include <numeric>
#include <algorithm>

#include <gloperate/offsetof.h>

#include <gloperate-text/GlyphSequence.h>
#include <gloperate-text/FontFace.h>


namespace
{
// http://stackoverflow.com/a/17074810 (thanks to Timothy Shields)

template <typename T, typename Compare>
std::vector<std::size_t> sort_permutation(
    const std::vector<T> & vec
,   const Compare & compare)
{
    std::vector<std::size_t> p(vec.size());

    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j) 
        { return compare(vec[i], vec[j]); });

    return p;
}

template <typename T>
std::vector<T> apply_permutation(
    const std::vector<T> & vec
,   const std::vector<std::size_t> & p)
{
    std::vector<T> sorted_vec(p.size());
    std::transform(p.begin(), p.end(), sorted_vec.begin(),
        [&](std::size_t i) { return vec[i]; });

    return sorted_vec;
}

}


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

void GlyphVertexCloud::update(const Vertices & vertices)
{
    if (!m_drawable)
        m_drawable = createDrawable();

    m_drawable->setVertices(vertices);
}

void GlyphVertexCloud::optimize(
    const std::vector<GlyphSequence> & sequences
,   const FontFace & fontFace)
{
    // L1/texture-cache optimization: sort vertex cloud by glyphs

    // create string associated with all depictable glyphs
    auto depictableChars = std::vector<char32_t>();
    auto vertices = m_vertices;

    for (const auto & sequence : sequences)
        sequence.chars(depictableChars, fontFace);

    assert(vertices.size() == depictableChars.size());

    const auto p = sort_permutation(depictableChars,
        [](const char32_t & a, const char32_t & b) { return a < b; });

    update(apply_permutation(vertices, p));
}


} // namespace gloperate_text
