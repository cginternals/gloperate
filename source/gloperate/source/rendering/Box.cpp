
#include <gloperate/rendering/Box.h>

#include <glm/glm.hpp>

#include <cppassist/memory/make_unique.h>
#include <cppassist/logging/logging.h>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>


using namespace cppassist;
using namespace gl;


namespace gloperate
{


Box::Box(glm::vec3 center, float size)
{
    const std::array<glm::vec3, 24> vertices { {
          center + glm::vec3(-0.5f * size, -0.5f * size,  0.5f * size)
        , center + glm::vec3( 0.5f * size, -0.5f * size,  0.5f * size)
        , center + glm::vec3(-0.5f * size,  0.5f * size,  0.5f * size)
        , center + glm::vec3( 0.5f * size,  0.5f * size,  0.5f * size)

        , center + glm::vec3(-0.5f * size, -0.5f * size, -0.5f * size)
        , center + glm::vec3(-0.5f * size,  0.5f * size, -0.5f * size)
        , center + glm::vec3( 0.5f * size, -0.5f * size, -0.5f * size)
        , center + glm::vec3( 0.5f * size,  0.5f * size, -0.5f * size)

        , center + glm::vec3(-0.5f * size, -0.5f * size,  0.5f * size)
        , center + glm::vec3(-0.5f * size,  0.5f * size,  0.5f * size)
        , center + glm::vec3(-0.5f * size, -0.5f * size, -0.5f * size)
        , center + glm::vec3(-0.5f * size,  0.5f * size, -0.5f * size)

        , center + glm::vec3( 0.5f * size, -0.5f * size, -0.5f * size)
        , center + glm::vec3( 0.5f * size,  0.5f * size, -0.5f * size)
        , center + glm::vec3( 0.5f * size, -0.5f * size,  0.5f * size)
        , center + glm::vec3( 0.5f * size,  0.5f * size,  0.5f * size)

        , center + glm::vec3(-0.5f * size,  0.5f * size,  0.5f * size)
        , center + glm::vec3( 0.5f * size,  0.5f * size,  0.5f * size)
        , center + glm::vec3(-0.5f * size,  0.5f * size, -0.5f * size)
        , center + glm::vec3( 0.5f * size,  0.5f * size, -0.5f * size)

        , center + glm::vec3(-0.5f * size, -0.5f * size,  0.5f * size)
        , center + glm::vec3(-0.5f * size, -0.5f * size, -0.5f * size)
        , center + glm::vec3( 0.5f * size, -0.5f * size,  0.5f * size)
        , center + glm::vec3( 0.5f * size, -0.5f * size, -0.5f * size)
    } };

    const std::array<glm::vec2, 24> texCoords { {
          glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(1.0f, 1.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)

        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 0.0f)
        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(1.0f, 1.0f)

        , glm::vec2(0.0f, 1.0f)
        , glm::vec2(0.0f, 0.0f)
        , glm::vec2(1.0f, 1.0f)
        , glm::vec2(1.0f, 0.0f)
    } };

    // Create vertex buffer
    m_vertices = make_unique<globjects::Buffer>();
    m_vertices->setData(vertices, gl::GL_STATIC_DRAW);

    // Create texture coordinate buffer
    m_texCoords = make_unique<globjects::Buffer>();
    m_texCoords->setData(texCoords, gl::GL_STATIC_DRAW);

    // Create vertex array object
    m_vao = make_unique<globjects::VertexArray>();
    m_vao->bind();
    {
        auto binding = m_vao->binding(0);
        binding->setAttribute(0);
        binding->setBuffer(m_vertices.get(), 0, sizeof(glm::vec3));
        binding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
        m_vao->enable(0);
    }
    {
        auto binding = m_vao->binding(1);
        binding->setAttribute(1);
        binding->setBuffer(m_texCoords.get(), 0, sizeof(glm::vec2));
        binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
        m_vao->enable(1);
    }

    // Done
    m_vao->unbind();
}

Box::~Box()
{
}

void Box::draw() const
{
    // Draw triangles
    m_vao->bind();
    m_vao->drawArrays(GL_TRIANGLE_STRIP,  0, 4);
    m_vao->drawArrays(GL_TRIANGLE_STRIP,  4, 4);
    m_vao->drawArrays(GL_TRIANGLE_STRIP,  8, 4);
    m_vao->drawArrays(GL_TRIANGLE_STRIP, 12, 4);
    m_vao->drawArrays(GL_TRIANGLE_STRIP, 16, 4);
    m_vao->drawArrays(GL_TRIANGLE_STRIP, 20, 4);
    m_vao->unbind();
}


} // namespace gloperate
