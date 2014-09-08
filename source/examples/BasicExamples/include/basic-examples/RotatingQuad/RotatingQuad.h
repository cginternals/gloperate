#pragma once


#include <globjects-base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <gloperate/Painter.h>
#include <gloperate/Camera.h>
#include <basic-examples/basic_examples_api.h>


namespace gloperate {
    class TextureLoader;
}


class BASIC_EXAMPLES_API RotatingQuad : public gloperate::Painter
{


public:
    RotatingQuad(gloperate::TextureLoader * loader = nullptr);
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
    glo::ref_ptr<gloperate::Camera> m_camera;
    glo::ref_ptr<glo::VertexArray>  m_vao;
    glo::ref_ptr<glo::Buffer>       m_buffer;
    glo::ref_ptr<glo::Program>      m_program;
    glo::ref_ptr<glo::Shader>       m_vertexShader;
    glo::ref_ptr<glo::Shader>       m_fragmentShader;
    glo::ref_ptr<glo::Texture>      m_texture;
    gloperate::TextureLoader      * m_textureLoader;
    float                           m_angle;


};
