#pragma once

#include <glm/mat4x4.hpp>

#include <gloperate/Painter.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>

#include <basic-painters/basic_painters_api.h>


namespace gloperate 
{
    //class Camera;
    class ResourceManager;
    class TargetFramebufferCapability;
    class ViewportCapability;
    class PerspectiveProjectionCapability;
    class CameraCapability;
    class TypedRenderTargetCapability;
    class VirtualTimeCapability;
}

class BASIC_PAINTERS_API CubeScape : public gloperate::Painter
{
public:
    CubeScape(gloperate::ResourceManager & resourceManager);
    virtual ~CubeScape();

    void update(float delta);

    void setupProjection();

    int numberOfCubes() const;
    void setNumberOfCubes(const int & number);

    bool animation() const;
    void setAnimation(const bool & enabled);
protected:
    virtual void onInitialize();
    virtual void onPaint();
    virtual void onTargetFramebufferChanged();
protected:
    /* parameters */

    int m_numCubes;
    bool m_animation;

    /* capabilities */
    gloperate::TargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::ViewportCapability * m_viewportCapability;
    gloperate::PerspectiveProjectionCapability * m_projectionCapability;
    gloperate::TypedRenderTargetCapability * m_typedRenderTargetCapability;
    gloperate::CameraCapability * m_cameraCapability;
    gloperate::VirtualTimeCapability * m_timeCapability;

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
};
