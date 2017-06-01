
#include <gloperate-text/GlyphVertexCloud.h>

#include <numeric>
#include <algorithm>

#include <cppassist/memory/offsetof.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/Drawable.h>

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


} // namespace


namespace gloperate_text
{


GlyphVertexCloud::GlyphVertexCloud()
: m_drawable(cppassist::make_unique<gloperate::Drawable>()),
  m_buffer(cppassist::make_unique<globjects::Buffer>())
{
    m_drawable->setPrimitiveMode(gl::GL_POINTS);
    m_drawable->setDrawMode(gloperate::DrawMode::Arrays);

    m_drawable->bindAttributes({ 0, 1, 2, 3, 4 });

    m_drawable->setBuffer(0, m_buffer.get());
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(Vertex));
    m_drawable->setAttributeBindingBuffer(1, 0, 0, sizeof(Vertex));
    m_drawable->setAttributeBindingBuffer(2, 0, 0, sizeof(Vertex));
    m_drawable->setAttributeBindingBuffer(3, 0, 0, sizeof(Vertex));
    m_drawable->setAttributeBindingBuffer(4, 0, 0, sizeof(Vertex));

    m_drawable->setAttributeBindingFormat(0, 3, gl::GL_FLOAT, gl::GL_FALSE, cppassist::offset(&Vertex::origin));
    m_drawable->setAttributeBindingFormat(1, 3, gl::GL_FLOAT, gl::GL_FALSE, cppassist::offset(&Vertex::vtan));
    m_drawable->setAttributeBindingFormat(2, 3, gl::GL_FLOAT, gl::GL_FALSE, cppassist::offset(&Vertex::vbitan));
    m_drawable->setAttributeBindingFormat(3, 4, gl::GL_FLOAT, gl::GL_FALSE, cppassist::offset(&Vertex::uvRect));
    m_drawable->setAttributeBindingFormat(4, 4, gl::GL_FLOAT, gl::GL_FALSE, cppassist::offset(&Vertex::fontColor));

    m_drawable->enableAllAttributeBindings();
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

gloperate::Drawable * GlyphVertexCloud::drawable()
{
    return m_drawable.get();
}

const gloperate::Drawable * GlyphVertexCloud::drawable() const
{
    return m_drawable.get();
}

GlyphVertexCloud::Vertices & GlyphVertexCloud::vertices()
{
    return m_vertices;
}

const GlyphVertexCloud::Vertices & GlyphVertexCloud::vertices() const
{
    return m_vertices;
}

void GlyphVertexCloud::update()
{
    m_drawable->buffer(0)->setData(m_vertices, gl::GL_STATIC_DRAW);
    m_drawable->setSize(m_vertices.size());
}

void GlyphVertexCloud::update(const Vertices & vertices)
{
    m_drawable->buffer(0)->setData(vertices, gl::GL_STATIC_DRAW);
    m_drawable->setSize(vertices.size());
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
