#pragma once

#include <array>

#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <globjects-base/ref_ptr.h>
#include <globjects-base/Referenced.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

#include <cgutils/cgutils_api.h>

namespace cgutils 
{

class CGUTILS_API UnitCube : public glo::Referenced
{
    static const std::array<glm::vec3, 28> strip();

public:
    UnitCube(gl::GLuint vertexAttribLocation = 0, gl::GLuint normalAttribLocation = 1);

    /** draws the unit cube as single triangle strip. Note: interleaved array 
        is used, so the normal comes from the provoking vertex (expected to be
        the default gl::glLAST_VERTEX_CONVENTION). use flat interpolation to get 
        the face normal within the fragment shader.
    */
    void draw();

private:
    glo::ref_ptr<glo::Buffer> m_strip;
    glo::ref_ptr<glo::VertexArray> m_vao;
};

} // namespace gloutils
