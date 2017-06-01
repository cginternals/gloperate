
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class Framebuffer;
    class Texture;
    class Program;
    class Shader;
    class AbstractStringSource;
}

namespace gloperate
{
    class Camera;
    class Box;
}


/**
*  @brief
*    Demo stage that renders a rotating box
*/
class DemoStage2 : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoStage2, gloperate::Stage
      , "RenderStage Demo" // Tags
      , ""                 // Icon
      , ""                 // Annotations
      , "Demo stage that renders a rotating box"
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
    DemoStage2(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoStage2();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    // Rendering data
    std::unique_ptr<globjects::Texture>              m_texture;
    std::unique_ptr<gloperate::Box>                  m_box;
    std::unique_ptr<globjects::Program>              m_program;
    std::unique_ptr<globjects::AbstractStringSource> m_vertexShaderSource;
    std::unique_ptr<globjects::AbstractStringSource> m_fragmentShaderSource;
    std::unique_ptr<globjects::Shader>               m_vertexShader;
    std::unique_ptr<globjects::Shader>               m_fragmentShader;
    std::unique_ptr<gloperate::Camera>               m_camera;
    float                                            m_angle;
};
