
#include <gloperate/rendering/Point.h>

#include <glm/glm.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>


namespace gloperate
{


Point::Point(cppassist::Flags<ShapeOption> options)
: Shape(ShapeType::Point, options)
{
    // Point geometry
    static const std::array<glm::vec2, 4> vertices { {
          glm::vec2(0.0f, 0.0f)
    } };

    static const std::array<glm::vec2, 4> texcoords { {
          glm::vec2(0.0f, 0.0f)
    } };

    // Create drawable
    m_drawable = cppassist::make_unique<Drawable>();
    m_drawable->setPrimitiveMode(gl::GL_POINTS);
    m_drawable->setDrawMode(DrawMode::Arrays);
    m_drawable->setSize(1);

    // Create vertex buffer
    m_vertices = cppassist::make_unique<globjects::Buffer>();
    m_vertices->setData(vertices, gl::GL_STATIC_DRAW);

    m_drawable->bindAttribute(0, 0);
    m_drawable->setBuffer(0, m_vertices.get());
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec2));
    m_drawable->setAttributeBindingFormat(0, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);
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

Point::~Point()
{
}

void Point::draw() const
{
    m_drawable->draw();
}


} // namespace gloperate
