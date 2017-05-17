
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/NamedString.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/Camera.h>


/**
*  @brief
*    Demo stage that applies SSAO to an image
*/
class DemoSSAOPostprocessingStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoSSAOPostprocessingStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that applies SSAO to an image"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface  renderInterface;  ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Texture *> colorTexture;     ///< Color texture of the scene
    Input<globjects::Texture *> normalTexture;    ///< Normal texture of the scene
    Input<globjects::Texture *> depthTexture;     ///< Depth texture of the scene

    Input<globjects::Texture *> ssaoKernel;       ///< SSAO kernel texture
    Input<globjects::Texture *> ssaoNoise;        ///< SSAO noise texture
    Input<glm::mat4>            projectionMatrix; ///< Projection matrix used for rendering the scene
    Input<glm::mat3>            normalMatrix;     ///< Normal matrix from scene rendering

    Input<bool>                 sceneRendered;    ///< Scene rendering stage processed?


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
    DemoSSAOPostprocessingStage(gloperate::Environment * environment, const std::string & name = "DemoSSAOPostprocessingStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAOPostprocessingStage();


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
    std::unique_ptr<globjects::Shader>      m_vertexShader;
    std::unique_ptr<globjects::Shader>      m_fragmentShader;
    std::unique_ptr<globjects::Program>     m_program;
    std::unique_ptr<globjects::NamedString> m_ssaoFileNamedString;
};
