
#include <gloperate/rendering/Icosahedron.h>

#include <iterator>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


using namespace glm;


namespace gloperate
{


std::array<vec3, 12> Icosahedron::baseVertices()
{
    static const float t = (1.f + glm::sqrt(5.f)) * 0.5f; // 2.118
    static const float i = glm::inversesqrt(t * t + 1.f);  // 0.427
    static const float a = t * i;                     // 0.904

    // icosahedron vertices (normalized) form three orthogonal golden rectangles
    // http://en.wikipedia.org/wiki/Icosahedron#Cartesian_coordinates

    return std::array<vec3, 12>{{
        vec3(-i, a, 0)
    ,   vec3( i, a, 0)
    ,   vec3(-i,-a, 0)
    ,   vec3( i,-a, 0)
    ,   vec3( 0,-i, a)
    ,   vec3( 0, i, a)
    ,   vec3( 0,-i,-a)
    ,   vec3( 0, i,-a)
    ,   vec3( a, 0,-i)
    ,   vec3( a, 0, i)
    ,   vec3(-a, 0,-i)
    ,   vec3(-a, 0, i)
    }};
}

std::array<Icosahedron::Face, 20> Icosahedron::baseIndices()
{
    return std::array<Face, 20>{{
        Face{{  0, 11,  5 }}
    ,   Face{{  0,  5,  1 }}
    ,   Face{{  0,  1,  7 }}
    ,   Face{{  0,  7, 10 }}
    ,   Face{{  0, 10, 11 }}

    ,   Face{{  1,  5,  9 }}
    ,   Face{{  5, 11,  4 }}
    ,   Face{{ 11, 10,  2 }}
    ,   Face{{ 10,  7,  6 }}
    ,   Face{{  7,  1,  8 }}

    ,   Face{{  3,  9,  4 }}
    ,   Face{{  3,  4,  2 }}
    ,   Face{{  3,  2,  6 }}
    ,   Face{{  3,  6,  8 }}
    ,   Face{{  3,  8,  9 }}

    ,   Face{{  4,  9,  5 }}
    ,   Face{{  2,  4, 11 }}
    ,   Face{{  6,  2, 10 }}
    ,   Face{{  8,  6,  7 }}
    ,   Face{{  9,  8,  1 }}
    }};
}

Icosahedron::Icosahedron()
{
}

Icosahedron::~Icosahedron()
{
}

void Icosahedron::generateGeometry(gl::GLsizei iterations)
{
    auto v(baseVertices());
    auto i(baseIndices());

    m_vertices = std::vector<vec3>(v.begin(), v.end());
    m_indices  = std::vector<Face>(i.begin(), i.end());

    refine(m_vertices, m_indices, static_cast<char>(glm::clamp(iterations, 0, 8)));
}

void Icosahedron::generateTextureCoordinates()
{
    m_texcoords.clear();
    m_texcoords.reserve(m_vertices.size());

    float ofs = 0.5;

    for (auto pos : m_vertices)
    {
        glm::vec3 normal = normalize(pos);
        glm::vec2 uv = glm::vec2(
            ofs - (glm::atan(normal.z, normal.x) / (2.0 * glm::pi<float>())),
            glm::asin(normal.y) / glm::pi<float>() + ofs
        );

        m_texcoords.push_back(uv);
    }
}

const std::vector<glm::vec3> & Icosahedron::vertices() const
{
    return m_vertices;
}

const std::vector<glm::vec2> & Icosahedron::texcoords() const
{
    return m_texcoords;
}

const std::vector<std::array<gl::GLushort, 3>> & Icosahedron::indices() const
{
    return m_indices;
}

void Icosahedron::refine(
    std::vector<vec3> & vertices
,   std::vector<Face> & indices
,   const unsigned char levels)
{
    std::unordered_map<uint, gl::GLushort> cache;

    for(int i = 0; i < levels; ++i)
    {
        const int size(static_cast<int>(indices.size()));

        for(int f = 0; f < size; ++f)
        {
            Face & face = indices[f];

            const gl::GLushort a(face[0]);
            const gl::GLushort b(face[1]);
            const gl::GLushort c(face[2]);

            const gl::GLushort ab(split(a, b, vertices, cache));
            const gl::GLushort bc(split(b, c, vertices, cache));
            const gl::GLushort ca(split(c, a, vertices, cache));

            face = {{ ab, bc, ca }};

            indices.emplace_back(Face{{ a, ab, ca }});
            indices.emplace_back(Face{{ b, bc, ab }});
            indices.emplace_back(Face{{ c, ca, bc }});
        }
    }
}

gl::GLushort Icosahedron::split(
    const gl::GLushort a
,   const gl::GLushort b
,   std::vector<vec3> & points
,   std::unordered_map<uint, gl::GLushort> & cache)
{
    const bool aSmaller(a < b);

    const uint smaller(aSmaller ? a : b);
    const uint greater(aSmaller ? b : a);
    const uint hash((smaller << 16) + greater);

    auto h(cache.find(hash));
    if(cache.end() != h)
        return h->second;

    points.push_back(normalize((points[a] + points[b]) * 0.5f));

    const gl::GLushort i = static_cast<gl::GLushort>(points.size() - 1);

    cache[hash] = i;

    return i;
}


} // namespace gloperate
