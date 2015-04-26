
#include <gloperate-assimp/PolygonalDrawable.h>

#include <glm/glm.hpp>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate-assimp/PolygonalGeometry.h>


using namespace gl;
namespace gloperate_assimp
{


PolygonalDrawable::PolygonalDrawable(const PolygonalGeometry & geometry)
{
    m_indices = new globjects::Buffer{};
    m_indices->setData(geometry.indices(), GL_STATIC_DRAW);

    m_size = static_cast<gl::GLsizei>(geometry.indices().size());

    m_vertices = new globjects::Buffer{};
    m_vertices->setData(geometry.vertices(), GL_STATIC_DRAW);

    if (geometry.hasNormals())
    {
        m_normals = new globjects::Buffer{};
        m_normals->setData(geometry.normals(), GL_STATIC_DRAW);
    }

    m_vao = new globjects::VertexArray{};
    m_vao->bind();

    m_indices->bind(GL_ELEMENT_ARRAY_BUFFER);

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(m_vertices, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, gl::GL_FLOAT);
    m_vao->enable(0);

    if (geometry.hasNormals())
    {
        auto vertexBinding = m_vao->binding(1);
        vertexBinding->setAttribute(1);
        vertexBinding->setBuffer(m_normals, 0, sizeof(glm::vec3));
        vertexBinding->setFormat(3, gl::GL_FLOAT, GL_TRUE);
        m_vao->enable(1);
    }

    m_vao->unbind();
}

void PolygonalDrawable::draw()
{
    m_vao->bind();
    m_vao->drawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, nullptr);
    m_vao->unbind();
}


} // namespace gloperate_assimp
