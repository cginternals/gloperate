#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <gloperate/Painter.h>
#include <gloperate/Camera.h>
#include <basic-examples/basic_examples_api.h>


namespace gloperate {
    class ResourceManager;
}


class BASIC_EXAMPLES_API RotatingQuad : public gloperate::Painter
{


public:
    RotatingQuad(gloperate::ResourceManager * resourceManager = nullptr);
    virtual ~RotatingQuad();


protected:
    virtual void onInitialize();
    virtual void onResize(const gloperate::Viewport & viewport);
    virtual void onPaint();


protected:
    void createAndSetupCamera();
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    globjects::ref_ptr<gloperate::Camera> m_camera;
    globjects::ref_ptr<globjects::VertexArray>  m_vao;
    globjects::ref_ptr<globjects::Buffer>       m_buffer;
    globjects::ref_ptr<globjects::Program>      m_program;
    globjects::ref_ptr<globjects::Shader>       m_vertexShader;
    globjects::ref_ptr<globjects::Shader>       m_fragmentShader;
    globjects::ref_ptr<globjects::Texture>      m_texture;
    gloperate::ResourceManager    * m_resourceManager;
    float                           m_angle;


};
