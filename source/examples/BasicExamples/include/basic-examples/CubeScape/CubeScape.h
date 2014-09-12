#pragma once

#include <glm/mat4x4.hpp>

#include <gloperate/Painter.h>
#include <basic-examples/basic_examples_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>

namespace gloperate {
    class ResourceManager;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractVirtualTimeCapability;
}

class BASIC_EXAMPLES_API CubeScape : public gloperate::Painter
{
public:
    CubeScape(gloperate::ResourceManager * resourceManager = nullptr);
    virtual ~CubeScape();

    void update(float delta);

    int numberOfCubes() const;
    void setNumberOfCubes(const int & number);
protected:
    virtual void onInitialize();
    virtual void onPaint();
protected:
    gloperate::ResourceManager * m_resourceManager;

    /* parameters */

    int m_numCubes;

    /* capabilities */

    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractVirtualTimeCapability * m_timeCapability;

    /* members */

    gl::GLint a_vertex;
    gl::GLint u_transform;
    gl::GLint u_time;
    gl::GLint u_numcubes;

    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_indices;
    globjects::ref_ptr<globjects::Buffer> m_vertices;

    globjects::ref_ptr<globjects::Program> m_program;

    globjects::ref_ptr<globjects::Texture> m_textures[2];

    glm::mat4 m_view;
    glm::mat4 m_projection;
};
