
#pragma once


#include <reflectionzeug/base/FilePath.h>
#include <reflectionzeug/base/Color.h>

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
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractVirtualTimeCapability;
}


class Logo : public gloperate::Painter
{
public:
    Logo(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo);
    virtual ~Logo();

    bool animation() const;
    void setAnimation(const bool & enabled);

    reflectionzeug::Color background() const;
    void setBackground(const reflectionzeug::Color & color);

    reflectionzeug::FilePath texture() const;
    void setTexture(const reflectionzeug::FilePath & filename);


protected:
    virtual void onInitialize();
    virtual void onPaint();


protected:
    void createAndSetupCamera();
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    /* Parameters */
    bool                     m_animation;
    reflectionzeug::Color    m_background;
    reflectionzeug::FilePath m_textureFilename;

    /* Capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractVirtualTimeCapability * m_timeCapability;

    globjects::ref_ptr<gloperate::Camera>       m_camera;
    globjects::ref_ptr<globjects::VertexArray>  m_vao;
    globjects::ref_ptr<globjects::Buffer>       m_buffer;
    globjects::ref_ptr<globjects::Program>      m_program;
    globjects::ref_ptr<globjects::Shader>       m_vertexShader;
    globjects::ref_ptr<globjects::Shader>       m_fragmentShader;
    globjects::ref_ptr<globjects::Texture>      m_texture;

    /* Data */
    float m_angle;
};
