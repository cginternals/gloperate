
#pragma once


#include <glm/mat4x4.hpp>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/VertexArray.h>

#include <gloperate/painter/Painter.h>


namespace gloperate 
{
    class ResourceManager;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
    class TypedRenderTargetCapability;
    class AbstractVirtualTimeCapability;
}


class CubeScape : public gloperate::Painter
{
public:
    CubeScape(gloperate::ResourceManager & resourceManager, const reflectionzeug::Variant & pluginInfo);
    virtual ~CubeScape();

    void update(float delta);

    void setupProjection();

    bool animation() const;
    void setAnimation(const bool & enabled);

    int numberOfCubes() const;
    void setNumberOfCubes(const int & number);


protected:
    virtual void onInitialize();
    virtual void onPaint();
    virtual void onTargetFramebufferChanged();


protected:
    // Scripting API
    void randomize();


protected:
    /* Parameters */
    bool m_animation;
    int m_numCubes;

    /* Data path */
    std::string m_dataPath;

    /* Capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::TypedRenderTargetCapability * m_typedRenderTargetCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;
    gloperate::AbstractVirtualTimeCapability * m_timeCapability;

    /* Data */
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
