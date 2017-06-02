
#include <gloperate/rendering/Sphere.h>

#include <iostream>

#include <glm/glm.hpp>

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>

#include <globjects/Buffer.h>


namespace gloperate
{


Sphere::Sphere(float radius, cppassist::Flags<ShapeOption> options)
: Shape(ShapeType::Sphere, options)
{
    // Create icosahedron
    m_icosahedron = cppassist::make_unique<Icosahedron>();
    m_icosahedron->generateGeometry(5);

    // Create drawable
    m_drawable = cppassist::make_unique<Drawable>();
    m_drawable->setPrimitiveMode(gl::GL_TRIANGLES);
    m_drawable->setDrawMode(gloperate::DrawMode::ElementsIndexBuffer);
    m_drawable->setSize(m_icosahedron->indices().size() * std::tuple_size<Icosahedron::Face>::value);

    // Create vertex buffer
    auto vertices = m_icosahedron->vertices();

    for (auto & vertex : vertices)
    {
        vertex *= radius;
    }

    m_vertices = cppassist::make_unique<globjects::Buffer>();
    m_vertices->setData(vertices, gl::GL_STATIC_DRAW);

    m_drawable->bindAttribute(0, 0);
    m_drawable->setBuffer(0, m_vertices.get());
    m_drawable->setAttributeBindingBuffer(0, 0, 0, sizeof(glm::vec3));
    m_drawable->setAttributeBindingFormat(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_drawable->enableAttributeBinding(0);

    // Create texture coordinate buffer
    if (options & ShapeOption::IncludeTexCoords)
    {
        m_icosahedron->generateTextureCoordinates();

        m_texCoords = cppassist::make_unique<globjects::Buffer>();
        m_texCoords->setData(m_icosahedron->texcoords(), gl::GL_STATIC_DRAW);

        m_drawable->bindAttribute(1, 1);
        m_drawable->setBuffer(1, m_texCoords.get());
        m_drawable->setAttributeBindingBuffer(1, 1, 0, sizeof(glm::vec2));
        m_drawable->setAttributeBindingFormat(1, 2, gl::GL_FLOAT, gl::GL_FALSE, 0);
        m_drawable->enableAttributeBinding(1);
    }

    // Create index buffer
    m_indices = cppassist::make_unique<globjects::Buffer>();
    m_indices->setData(m_icosahedron->indices(), gl::GL_STATIC_DRAW);

    m_drawable->setIndexBuffer(m_indices.get(), gl::GL_UNSIGNED_SHORT);
}

Sphere::~Sphere()
{
}

void Sphere::draw() const
{
    m_drawable->draw();
}


} // namespace gloperate
