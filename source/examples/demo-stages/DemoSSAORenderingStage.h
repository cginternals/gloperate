
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <demo-stages/demo-stages_api.h>


/**
*  @brief
*    Demo stage that renders a simple scene onto the screen
*/
class DEMO_STAGES_API DemoSSAORenderingStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoSSAORenderingStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that renders a simple scene onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Framebuffer *> normalFBO;  ///< FBO to render normals into

    // Outputs
    Output<glm::mat4> projectionMatrix;         ///< Projection matrix used for rendering
    Output<glm::mat3> normalMatrix;             ///< Normal matrix used for rendering


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
    DemoSSAORenderingStage(gloperate::Environment * environment, const std::string & name = "DemoSSAORenderingStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAORenderingStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void setupGeometry();
    void setupProgram();


protected:
    // Rendering objects
    std::unique_ptr<globjects::VertexArray> m_vao;
    std::unique_ptr<globjects::Buffer>      m_vertexBuffer;
    std::unique_ptr<globjects::Program>     m_colorProgram;
    std::unique_ptr<globjects::Program>     m_normalProgram;
    std::unique_ptr<globjects::Shader>      m_vertexShader;
    std::unique_ptr<globjects::Shader>      m_colorFragmentShader;
    std::unique_ptr<globjects::Shader>      m_normalFragmentShader;
};
