
#pragma once


#include <vector>

#include <glm/fwd.hpp>

#include <gloperate-assimp/gloperate-assimp_api.h>


namespace gloperate_assimp
{


/**
*  @brief
*    Triangle mesh stored on the CPU
*
*  @remarks
*    This mesh class contains a list of triangles, stored on the CPU.
*    To upload on GPU and draw a mesh, use PolygonalDrawable.
*/
class GLOPERATE_ASSIMP_API PolygonalGeometry
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


protected:
    std::vector<unsigned int> m_indices;    /**< Index array */
    std::vector<glm::vec3>    m_vertices;   /**< Vertex array */
    std::vector<glm::vec3>    m_normals;    /**< Normal array */
};


} // namespace gloperate_assimp
