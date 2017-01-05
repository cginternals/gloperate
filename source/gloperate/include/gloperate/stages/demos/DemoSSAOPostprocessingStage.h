
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/Camera.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that applies SSAO to an image
*/
class GLOPERATE_API DemoSSAOPostprocessingStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoSSAOPostprocessingStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that applies SSAO to an image"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface renderInterface; ///< Interface for rendering into a viewer

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
    DemoSSAOPostprocessingStage(Environment * environment, const std::string & name = "DemoSSAOPostprocessingStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAOPostprocessingStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void setupGeometry();
    void setupProgram();


protected:
    // Rendering objects
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;
    globjects::ref_ptr<globjects::Program>     m_program;
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;
    globjects::ref_ptr<globjects::Shader>      m_fragmentShader;
};


} // namespace gloperate
