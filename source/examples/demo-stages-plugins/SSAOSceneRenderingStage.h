
#pragma once


#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


/**
*  @brief
*    Demo stage that renders a simple scene onto the screen
*/
class SSAOSceneRenderingStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        SSAOSceneRenderingStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that renders a simple scene onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

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
    SSAOSceneRenderingStage(gloperate::Environment * environment, const std::string & name = "SSAOSceneRenderingStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~SSAOSceneRenderingStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;

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
};
