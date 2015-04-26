
#pragma once


#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate-assimp/gloperate-assimp_api.h>


namespace globjects
{
    class Buffer;
    class VertexArray;
}


namespace gloperate_assimp
{


class PolygonalGeometry;


class GLOPERATE_ASSIMP_API PolygonalDrawable
{
public:
    PolygonalDrawable(const PolygonalGeometry & geometry);

    void draw();

protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_indices;
    globjects::ref_ptr<globjects::Buffer> m_vertices;
    globjects::ref_ptr<globjects::Buffer> m_normals;
    gl::GLsizei m_size;
};


} // namespace gloperate_assimp
