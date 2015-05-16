
#include <gloperate/primitives/PolygonalGeometry.h>

#include <glm/glm.hpp>


namespace gloperate
{


PolygonalGeometry::PolygonalGeometry()
{
}

PolygonalGeometry::~PolygonalGeometry()
{
}

const std::vector<unsigned int> & PolygonalGeometry::indices() const
{
    return m_indices;
}

void PolygonalGeometry::setIndices(const std::vector<unsigned int> & indices)
{
    m_indices = indices;
}

void PolygonalGeometry::setIndices(std::vector<unsigned int> && indices)
{
    m_indices = std::move(indices);
}

const std::vector<glm::vec3> & PolygonalGeometry::vertices() const
{
    return m_vertices;
}

void PolygonalGeometry::setVertices(const std::vector<glm::vec3> & vertices)
{
    m_vertices = vertices;
}

void PolygonalGeometry::setVertices(std::vector<glm::vec3> && vertices)
{
    m_vertices = std::move(vertices);
}

bool PolygonalGeometry::hasNormals() const
{
    return !m_normals.empty();
}

const std::vector<glm::vec3> & PolygonalGeometry::normals() const
{
    return m_normals;
}

void PolygonalGeometry::setNormals(const std::vector<glm::vec3> & normals)
{
    m_normals = normals;
}

void PolygonalGeometry::setNormals(std::vector<glm::vec3> && normals)
{
    m_normals = std::move(normals);
}

bool PolygonalGeometry::hasTextureCoordinates() const
{
	return !m_textureCoordinates.empty();
}

const std::vector<glm::vec3> & PolygonalGeometry::textureCoordinates() const
{
	return m_textureCoordinates;
}

void PolygonalGeometry::setTextureCoordinates(const std::vector<glm::vec3> & textureCoordinates)
{
	m_textureCoordinates = textureCoordinates;
}

void PolygonalGeometry::setTextureCoordinates(std::vector<glm::vec3> && textureCoordinates)
{
	m_textureCoordinates = std::move(textureCoordinates);
}

unsigned PolygonalGeometry::materialIndex() const
{
	return m_materialIndex;
}

void PolygonalGeometry::setMaterialIndex(const unsigned int materialIndex)
{
	m_materialIndex = materialIndex;
}

} // namespace gloperate
