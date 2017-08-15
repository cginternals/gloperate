
#pragma once


#include <array>
#include <vector>
#include <unordered_map>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Helper class to create an icosahedron geometry that can be refined dynamically
*/
class GLOPERATE_API Icosahedron
{
public:
    /**
    *  @brief
    *    Data type for faces
    */
    using Face = std::array<gl::GLushort, 3>;


public:
    /**
    *  @brief
    *    Obtain vertices of the original icosahedron geometry that can be refined
    *
    *  @return
    *    Vertex array
    */
    static std::array<glm::vec3, 12> baseVertices();

    /**
    *  @brief
    *    Obtain indices for the original icosahedron geometry that can be refined
    *
    *  @return
    *    Face array
    *
    *  @remarks
    *    The geometry is made up of triangles (no strip, no fan)
    */
    static std::array<Face, 20> baseIndices();

    /**
    *  @brief
    *    Refine geometry iteratively
    *
    *    Splits each triangle into 4 new ones and creates the appropriate points and indices
    *
    *  @param[in] vertices
    *    Vertex array
    *  @param[in] indices
    *    Face array
    *  @param[in] levels
    *    Number of levels
    */
    static void refine(std::vector<glm::vec3> & vertices, std::vector<Face> & indices, unsigned char levels);


public:
    /**
    *  @brief
    *    Constructor
    */
    Icosahedron();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Icosahedron();

    /**
    *  @brief
    *    Create and refine geometry
    *
    *  @param[in] iterations
    *    Number of refinement iterations
    */
    void generateGeometry(gl::GLsizei iterations = 0);

    /**
    *  @brief
    *    Generate texture coordinates
    */
    void generateTextureCoordinates();

    /**
    *  @brief
    *    Get vertices
    *
    *  @return
    *    Vertex array
    */
    const std::vector<glm::vec3> & vertices() const;

    /**
    *  @brief
    *    Get texture coordinates
    *
    *  @return
    *    Texture coordinate array
    *
    *  @remarks
    *    The function generateTextureCoordinates() has to be called before,
    *    otherwise the returned vector will be empty.
    */
    const std::vector<glm::vec2> & texcoords() const;

    /**
    *  @brief
    *    Get indices
    *
    *  @return
    *    Index array (describes a list of triangles)
    */
    const std::vector<std::array<gl::GLushort, 3>> & indices() const;


private:
    /**
    *  @brief
    *    Split triangle
    *
    *    Splits a triangle edge by adding an appropriate new point (normalized
    *    on sphere) to the points (if not already cached) and returns the index
    *    to this point.
    *
    *  @param[in] a
    *    Index of the first point in points
    *  @param[in] b
    *    Index of the second point in points
    *  @param[in] points
    *    Vertex array
    *  @param[in] cache
    *    Point cache
    *
    *  @return
    *    Index of the new point
    */
    static gl::GLushort split(
        gl::GLushort a
    ,   gl::GLushort b
    ,   std::vector<glm::vec3> & points
    ,   std::unordered_map<glm::uint, gl::GLushort> & cache);


private:
    std::vector<glm::vec3> m_vertices;  ///< Vertex array
    std::vector<glm::vec2> m_texcoords; ///< Texture coordinate array
    std::vector<Face>      m_indices;   ///< Index array
};


} // namespace gloperate
