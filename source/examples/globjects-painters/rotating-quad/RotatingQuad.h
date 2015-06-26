
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/painter/Camera.h>


namespace gloperate 
{
    class ResourceManager;
    class AbstractViewportCapability;
    class AbstractVirtualTimeCapability;
}


class RotatingQuad : public gloperate::Painter
{
public:
    RotatingQuad(gloperate::ResourceManager & resourceManager);
    virtual ~RotatingQuad();


protected:
    virtual void onInitialize();
    virtual void onPaint();


protected:
    void createAndSetupCamera();
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    /* capabilities */
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractVirtualTimeCapability * m_timeCapability;

    globjects::ref_ptr<gloperate::Camera>       m_camera;
    globjects::ref_ptr<globjects::VertexArray>  m_vao;
    globjects::ref_ptr<globjects::Buffer>       m_buffer;
    globjects::ref_ptr<globjects::Program>      m_program;
    globjects::ref_ptr<globjects::Shader>       m_vertexShader;
    globjects::ref_ptr<globjects::Shader>       m_fragmentShader;
    globjects::ref_ptr<globjects::Texture>      m_texture;

    float m_angle;
};
