#include <iostream>

#include <gloperate/primitives/PolygonalGeometry.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>


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

bool PolygonalGeometry::isRigged() const
{
    return !m_vertexBoneIndices.empty() &&
            !m_vertexBoneWeights.empty() &&
            m_vertexBoneIndices.size() == m_vertexBoneWeights.size();
}

const std::vector<glm::ivec4> &PolygonalGeometry::vertexBoneIndices() const
{
    return m_vertexBoneIndices;
}

void PolygonalGeometry::setVertexBoneIndices(const std::vector<glm::ivec4> & vertexBoneIndices)
{
    m_vertexBoneIndices = vertexBoneIndices;
}

void PolygonalGeometry::setVertexBoneIndices(std::vector<glm::ivec4> && vertexBoneIndices)
{
    m_vertexBoneIndices = std::move(vertexBoneIndices);
}

const std::vector<glm::vec4> & PolygonalGeometry::vertexBoneWeights() const
{
    return m_vertexBoneWeights;
}

void PolygonalGeometry::setVertexBoneWeights(const std::vector<glm::vec4> & vertexBoneWeights)
{
    m_vertexBoneWeights = vertexBoneWeights;
}

void PolygonalGeometry::setVertexBoneWeights(std::vector<glm::vec4> && vertexBoneWeights)
{
    m_vertexBoneWeights = std::move(vertexBoneWeights);
}

const std::map<std::string, size_t> & PolygonalGeometry::boneMapping() const
{
    return m_boneMapping;
}

void PolygonalGeometry::setBoneMapping(const std::map<std::string,size_t> & boneMapping)
{
    m_boneMapping = boneMapping;
}

void PolygonalGeometry::setBoneMapping(std::map<std::string, size_t>&& boneMapping)
{
    m_boneMapping = std::move(boneMapping);
}

const std::vector<glm::mat4> & PolygonalGeometry::bindTransforms() const
{
    return m_bindTransforms;
}

void PolygonalGeometry::setBindTransforms(const std::vector<glm::mat4> & bindTransforms)
{
    m_bindTransforms = bindTransforms;
}

void PolygonalGeometry::setBindTransforms(std::vector<glm::mat4> && bindTransforms)
{
    m_bindTransforms = std::move(bindTransforms);
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
