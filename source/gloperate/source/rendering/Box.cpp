
#include <gloperate/rendering/Box.h>

#include <glm/glm.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>


namespace gloperate
{


Box::Box(float size, cppassist::Flags<ShapeOption> options)
: Box(size, size, size, options)
{
}

Box::Box(float width, float height, float depth, cppassist::Flags<ShapeOption> options)
: Shape(ShapeType::Box, options)
{
    // Box geometry
    static const std::array<glm::vec3, 36> vertices { {
          glm::vec3(-0.5f, -0.5f,  0.5f)
        , glm::vec3( 0.5f, -0.5f,  0.5f)
        , glm::vec3(-0.5f,  0.5f,  0.5f)

        , glm::vec3( 0.5f, -0.5f,  0.5f)
        , glm::vec3( 0.5f,  0.5f,  0.5f)
        , glm::vec3(-0.5f,  0.5f,  0.5f)

        , glm::vec3(-0.5f, -0.5f, -0.5f)
        , glm::vec3(-0.5f,  0.5f, -0.5f)
        , glm::vec3( 0.5f, -0.5f, -0.5f)

        , glm::vec3(-0.5f,  0.5f, -0.5f)
        , glm::vec3( 0.5f,  0.5f, -0.5f)
        , glm::vec3( 0.5f, -0.5f, -0.5f)

        , glm::vec3(-0.5f, -0.5f,  0.5f)
        , glm::vec3(-0.5f,  0.5f,  0.5f)
        , glm::vec3(-0.5f, -0.5f, -0.5f)

        , glm::vec3(-0.5f,  0.5f,  0.5f)
        , glm::vec3(-0.5f,  0.5f, -0.5f)
        , glm::vec3(-0.5f, -0.5f, -0.5f)

        , glm::vec3( 0.5f, -0.5f, -0.5f)
        , glm::vec3( 0.5f,  0.5f, -0.5f)
        , glm::vec3( 0.5f, -0.5f,  0.5f)

        , glm::vec3( 0.5f,  0.5f, -0.5f)
        , glm::vec3( 0.5f,  0.5f,  0.5f)
        , glm::vec3( 0.5f, -0.5f,  0.5f)

        , glm::vec3(-0.5f,  0.5f,  0.5f)
        , glm::vec3( 0.5f,  0.5f,  0.5f)
        , glm::vec3(-0.5f,  0.5f, -0.5f)

        , glm::vec3( 0.5f,  0.5f,  0.5f)
        , glm::vec3( 0.5f,  0.5f, -0.5f)
        , glm::vec3(-0.5f,  0.5f, -0.5f)

        , glm::vec3(-0.5f, -0.5f,  0.5f)
        , glm::vec3(-0.5f, -0.5f, -0.5f)
        , glm::vec3( 0.5f, -0.5f,  0.5f)

        , glm::vec3(-0.5f, -0.5f, -0.5f)
        , glm::vec3( 0.5f, -0.5f, -0.5f)
        , glm::vec3( 0.5f, -0.5f,  0.5f)
    } };

    static const std::array<glm::vec2, 36> texcoords { {
          glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)

        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)

        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)

        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)

        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)

        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
    } };

    // Create drawable
    m_drawable = cppassist::make_unique<Drawable>();
    m_drawable->setPrimitiveMode(gl::GL_TRIANGLES);
    m_drawable->setDrawMode(DrawMode::Arrays);
    m_drawable->setSize(36);

    // Create vertex buffer
    auto v = vertices;

    for (auto & vertex : v) {
        vertex *= glm::vec3(width, height, depth);
    }

    m_vertices = cppassist::make_unique<globjects::Buffer>();
    m_vertices->setData(v, gl::GL_STATIC_DRAW);

    m_drawable->bindAttribute(0, 0);
    m_drawable->setBuffer(0, m_vertices.get());
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec3));
    m_drawable->setAttributeBindingFormat(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_drawable->enableAttributeBinding(0);

    // Create texture coordinate buffer
    if (options & ShapeOption::IncludeTexCoords)
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

Box::~Box()
{
}

void Box::draw() const
{
    m_drawable->draw();
}


} // namespace gloperate
