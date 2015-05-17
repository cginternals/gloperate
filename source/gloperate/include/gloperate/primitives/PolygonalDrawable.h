
#pragma once


#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/primitives/AbstractDrawable.h>


namespace globjects
{
    class Buffer;
    class VertexArray;
}


namespace gloperate
{


class PolygonalGeometry;


/**
*  @brief
*    Triangle mesh drawable
*
*  @remarks
*    This class is used to upload and draw a triangle mesh on the GPU.
*    To create or load meshes, see PolygonalGeometry and Loader<PolygonalGeometry>.
*/
class GLOPERATE_API PolygonalDrawable : public AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] geometry
    *    CPU mesh representation
    *
    *  @remarks
    *    The geometry is only used once to generate the mesh representation
    *    on the GPU and not used afterwards.
    */
    PolygonalDrawable(const PolygonalGeometry & geometry);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PolygonalDrawable();

    /**
    *  @brief
    *    Draw geometry
    *
    *  @remarks
    *    The geometry is drawn as an indexed geometry of type GL_TRIANGLES.
    */
    virtual void draw() override;


protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;                 /**< Vertex array object */
    globjects::ref_ptr<globjects::Buffer>      m_indices;             /**< Index buffer */
    globjects::ref_ptr<globjects::Buffer>      m_vertices;            /**< Vertex buffer */
    globjects::ref_ptr<globjects::Buffer>      m_normals;             /**< Normal buffer (may be empty) */
    globjects::ref_ptr<globjects::Buffer>      m_textureCoordinates;  /**< Texture coordinate buffer (may be empty) */
    gl::GLsizei                                m_size;                /**< Number of elements (m_indices) */
};


} // namespace gloperate
