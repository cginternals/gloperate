
#include <gloperate/primitives/PolygonalDrawable.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

#include <gloperate/primitives/PolygonalGeometry.h>


using namespace gl;


namespace gloperate
{


PolygonalDrawable::PolygonalDrawable(const PolygonalGeometry & geometry)
{
    // Create and copy index buffer
    m_indices = new globjects::Buffer;
    m_indices->setData(geometry.indices(), GL_STATIC_DRAW);

    // Save number of elements in index buffer
    m_size = static_cast<gl::GLsizei>(geometry.indices().size());

    // Create and copy vertex buffer
    m_vertices = new globjects::Buffer;
    m_vertices->setData(geometry.vertices(), GL_STATIC_DRAW);

    // Create and copy normal buffer
    if (geometry.hasNormals())
    {
        m_normals = new globjects::Buffer;
        m_normals->setData(geometry.normals(), GL_STATIC_DRAW);
    }

	// Create and copy texture coordinate buffer
	if (geometry.hasTextureCoordinates())
	{
		m_textureCoordinates = new globjects::Buffer;
		m_textureCoordinates->setData(geometry.textureCoordinates(), GL_STATIC_DRAW);
	}

    // Create vertex array object
    m_vao = new globjects::VertexArray;
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

	if (geometry.hasTextureCoordinates())
	{
		auto vertexBinding = m_vao->binding(2);
		vertexBinding->setAttribute(2);
		vertexBinding->setBuffer(m_textureCoordinates, 0, sizeof(glm::vec3));
		vertexBinding->setFormat(3, gl::GL_FLOAT);
		m_vao->enable(2);
	}	

    m_vao->unbind();
}

PolygonalDrawable::~PolygonalDrawable()
{
}

void PolygonalDrawable::draw()
{
    // Draw triangles
    m_vao->bind();
    m_vao->drawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, nullptr);
    m_vao->unbind();
}


} // namespace gloperate
