
#include <gloperate/rendering/Quad.h>

#include <glm/glm.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>


namespace gloperate
{


Quad::Quad(float size, unsigned int options)
: Quad(size, size, options)
{
}

Quad::Quad(float width, float height, unsigned int options)
: Shape(ShapeType::Quad, options)
{
    // Quad geometry
    static const std::array<glm::vec2, 4> vertices { {
          glm::vec2(-0.5f, -0.5f)
        , glm::vec2( 0.5f, -0.5f)
        , glm::vec2(-0.5f,  0.5f)
        , glm::vec2( 0.5f,  0.5f)
    } };

    static const std::array<glm::vec2, 4> texcoords { {
          glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(1.0f, 1.0f)
    } };

    // Check options
    bool createTexCoords = ((options & (int)ShapeOption::IncludeTexCoords) != 0);

    // Create drawable
    m_drawable = cppassist::make_unique<Drawable>();
    m_drawable->setPrimitiveMode(gl::GL_TRIANGLE_STRIP);
    m_drawable->setDrawMode(DrawMode::Arrays);
    m_drawable->setSize(4);

    // Create vertex buffer
    auto v = vertices;

    for(auto & vertex : v) {
        vertex *= glm::vec2(width, height);
    }

    m_vertices = cppassist::make_unique<globjects::Buffer>();
    m_vertices->setData(v, gl::GL_STATIC_DRAW);

    m_drawable->bindAttribute(0, 0);
    m_drawable->setBuffer(0, m_vertices.get());
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec2));
    m_drawable->setAttributeBindingFormat(0, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_drawable->enableAttributeBinding(0);

    // Create texture coordinate buffer
    if (createTexCoords)
    {
        m_texCoords = cppassist::make_unique<globjects::Buffer>();
        m_texCoords->setData(texcoords, gl::GL_STATIC_DRAW);

        m_drawable->bindAttribute(1, 1);
        m_drawable->setBuffer(1, m_texCoords.get());
        m_drawable->setAttributeBindingBuffer(1, 1, 0, sizeof(glm::vec2));
        m_drawable->setAttributeBindingFormat(1, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);
        m_drawable->enableAttributeBinding(1);
    }
}

Quad::~Quad()
{
}

void Quad::draw() const
{
    m_drawable->draw();
}


} // namespace gloperate
