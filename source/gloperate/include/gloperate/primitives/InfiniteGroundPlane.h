#pragma once

#include <glm/fwd.hpp>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/Referenced.h>

#include <gloperate/primitives/VertexDrawable.h>
#include <gloperate/gloperate_api.h>

namespace globjects
{
    class Shader;
    class Program;
    class VertexDrawable;
}

namespace gloperate
{

    /**
     *  @brief
     *    Infinite plane
     *
     *    It is only drawn into the depth buffer by default, a fragment shader can by supplied to change this behaviour.
     */
    class GLOPERATE_API InfiniteGroundPlane : public globjects::Referenced
    {
    public:
        InfiniteGroundPlane();
        InfiniteGroundPlane(globjects::Shader * fragmentShader);

        void draw(const glm::mat4 &viewProjectionMatrix);
        globjects::Program * program() const;

    protected:
        globjects::ref_ptr<globjects::Program> m_program;
        globjects::ref_ptr<gloperate::VertexDrawable> m_drawable;

    protected:
        static const char * s_defaultVertexShaderSource;
        static const char * s_defaultFragmentShaderSource;
    };
}
