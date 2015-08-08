#pragma once


#include <vector>
#include <map>
#include <string>

#include <glm/fwd.hpp>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Triangle mesh stored on the CPU
*
*  @remarks
*    This mesh class contains a list of triangles, stored on the CPU.
*    To upload on GPU and draw a mesh, use PolygonalDrawable.
*/
class GLOPERATE_API PolygonalGeometry
{
public:
    /**
    *  @brief
    *    Constructor
    */
    PolygonalGeometry();

    /**
    *  @brief
    *    Destructor
    */
    ~PolygonalGeometry();

    /**
    *  @brief
    *    Get index array
    *
    *  @return
    *    Index array
    */
    const std::vector<unsigned int> & indices() const;

    /**
    *  @brief
    *    Set index array
    *
    *  @param[in] indices
    *    Index array
    */
    void setIndices(const std::vector<unsigned int> & indices);

    /**
    *  @brief
    *    Set index array
    *
    *  @param[in] indices
    *    Index array
    */
    void setIndices(std::vector<unsigned int> && indices);

    /**
    *  @brief
    *    Get vertex array
    *
    *  @return
    *    Vertex array
    */
    const std::vector<glm::vec3> & vertices() const;

    /**
    *  @brief
    *    Set vertex array
    *
    *  @param[in] vertices
    *    Vertex array
    */
    void setVertices(const std::vector<glm::vec3> & vertices);

    /**
    *  @brief
    *    Set vertex array
    *
    *  @param[in] vertices
    *    Vertex array
    */
    void setVertices(std::vector<glm::vec3> && vertices);

    /**
    *  @brief
    *    Check if mesh contains normal vectors
    *
    *  @return
    *    'true' if the mesh contains normals, else 'false'
    */
    bool hasNormals() const;

    /**
    *  @brief
    *    Get normal array
    *
    *  @return
    *    Normal array
    */
    const std::vector<glm::vec3> & normals() const;

    /**
    *  @brief
    *    Set normal array
    *
    *  @param[in] normals
    *    Normal array
    */
    void setNormals(const std::vector<glm::vec3> & normals);

    /**
    *  @brief
    *    Set normal array
    *
    *  @param[in] normals
    *    Normal array
    */
    void setNormals(std::vector<glm::vec3> && normals);

	/**
	*  @brief
	*    Check if mesh contains texture coordinates
	*
	*  @return
	*    'true' if the mesh contains texture coordinates, otherwise 'false'
	*/
	bool hasTextureCoordinates() const;

	/**
	*  @brief
	*    Get texture coordinate array
	*
	*  @return
	*    Texture coordinate array
	*/
	const std::vector<glm::vec3> & textureCoordinates() const;

	/**
	*  @brief
	*    Set texture coordinates
	*
	*  @param[in] textureCoordinates
	*    Texture coordinate array
	*/
	void setTextureCoordinates(const std::vector<glm::vec3> & textureCoordinates);

	/**
	*  @brief
	*    Set texture coordinates
	*
	*  @param[in] textureCoordinates
	*    Texture coordinate array
	*/
	void setTextureCoordinates(std::vector<glm::vec3> && textureCoordinates);

    /**
    *  @brief
    *    Check if mesh is rigged
    *
    *  @return
    *    'true' if the mesh is rigged, else 'false'
    */
    bool isRigged() const;

    /**
    *  @brief
    *    Get the bone indices of each vertex
    *
    *  @return
    *    Bone indices of each vertex
    */
    const std::vector<glm::ivec4> &vertexBoneIndices() const;

    /**
    *  @brief
    *    Set the bone indices of each vertex
    *
    *  @param[in] vertexBoneIndices
    *    vertexBoneIndices array
    */
    void setVertexBoneIndices(const std::vector<glm::ivec4> & vertexBoneIndices);

    /**
    *  @brief
    *    Set the bone indices of each vertex
    *
    *  @param[in] vertexBoneIndices
    *    vertexBoneIndices array
    */
    void setVertexBoneIndices(std::vector<glm::ivec4> && vertexBoneIndices);

    /**
    *  @brief
    *    Get the bone weights per vertex
    *
    *  @return
    *    Array of the bone weights per vertex
    */
    const std::vector<glm::vec4> & vertexBoneWeights() const;

    /**
    *  @brief
    *    Set the bone weights per vertex
    *
    *  @param[in] vertexBoneWeights
    *    Array of the bone weights per vertex
    */
    void setVertexBoneWeights(const std::vector<glm::vec4> & vertexBoneWeights);

    /**
    *  @brief
    *    Set the bone weights per vertex
    *
    *  @param[in] vertexBoneWeights
    *    Array of the bone weights per vertex
    */
    void setVertexBoneWeights(std::vector<glm::vec4> && vertexBoneWeights);

    /**
    *  @brief
    *    Get the bone mapping
    *
    *  @return
    *    Mapping from bone name to bone index
    */
    const std::map<std::string, size_t> & boneMapping() const;

    /**
    *  @brief
    *    Set the bone mapping
    *
    *  @param[in] boneMapping
    *    Mapping from bone name to bone index
    */
    void setBoneMapping(const std::map<std::string, size_t> &boneMapping);

    /**
    *  @brief
    *    Set the bone mapping
    *
    *  @param[in] boneMapping
    *    Mapping from bone name to bone index
    */
    void setBoneMapping(std::map<std::string, size_t> &&boneMapping);

    /**
    *  @brief
    *    Get the bind Transforms
    *
    *  @return
    *    bindTransforms
    */
    const std::vector<glm::mat4> & bindTransforms() const;

    /**
    *  @brief
    *    Set the bind Transforms
    *
    *  @param[in] bindTransforms
    *    Array of bind transforms
    */
    void setBindTransforms(const std::vector<glm::mat4> & bindTransforms);

    /**
    *  @brief
    *    Set the bind Transforms
    *
    *  @param[in] bindTransforms
    *    Array of bind transforms
    */
    void setBindTransforms(std::vector<glm::mat4> &&bindTransforms);

    /**
	*  @brief
	*    Get material index
	*
	*  @return
	*    Material index
	*/
	unsigned int materialIndex() const;

	/**
	*  @brief
	*    Set material index
	*
	*  @param[in] materialIndex
	*    Material index
	*/
	void setMaterialIndex(unsigned int materialIndex);


protected:
    std::vector<unsigned int> m_indices;              /**< Index array */
    std::vector<glm::vec3>    m_vertices;             /**< Vertex array */
    std::vector<glm::vec3>    m_normals;              /**< Normal array */
    std::vector<glm::vec3>    m_textureCoordinates;   /**< Texture coordinate array */
    std::vector<glm::ivec4>   m_vertexBoneIndices;    /**< Indices per index > */
    std::vector<glm::vec4>    m_vertexBoneWeights;    /**< Weights per index > */
    std::map<std::string, size_t>  m_boneMapping;     /**< Mapping from string to boneIndex > */
    std::vector<glm::mat4>    m_bindTransforms;       /**< bind Matrices per bone > */
    unsigned int              m_materialIndex;        /**< Material index */
};


} // namespace gloperate
