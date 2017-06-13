
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/base/StringTemplate.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/rendering/Sphere.h>


/**
*  @brief
*    Demo stage that renders a spinning rectangle onto the screen
*/
class DemoStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoStage, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo stage that renders a spinning rectangle onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Outputs
    Output<globjects::Framebuffer *> fboOut; ///< Pass through of target FBO


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    DemoStage(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;

    // Helper functions
    void createAndSetupCamera();
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    // Rendering objects
    gloperate::Camera                   m_camera;
    std::unique_ptr<gloperate::Sphere>  m_quad;
    std::unique_ptr<globjects::Texture> m_texture;
    std::unique_ptr<globjects::AbstractStringSource>  m_vertexShaderSource;
    std::unique_ptr<globjects::AbstractStringSource>  m_fragmentShaderSource;
    std::unique_ptr<globjects::Shader>                m_vertexShader;
    std::unique_ptr<globjects::Shader>                m_fragmentShader;
    std::unique_ptr<globjects::Program>               m_program;

    // Status
    float m_angle;  ///< Current angle of rotating triangle (in radians)
};
