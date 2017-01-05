
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


namespace gloperate
{


/**
*  @brief
*    Demo stage that renders a simple scene onto the screen
*/
class GLOPERATE_API DemoSSAORenderingStage : public Stage
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
    RenderInterface renderInterface;           ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Framebuffer *> normalFBO; ///< FBO to render normals into

    // Outputs
    Output<glm::mat4> projectionMatrix;        ///< Projection matrix used for rendering
    Output<glm::mat3> normalMatrix;            ///< Normal matrix used for rendering


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
    DemoSSAORenderingStage(Environment * environment, const std::string & name = "DemoSSAORenderingStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAORenderingStage();


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
    globjects::ref_ptr<globjects::Program>     m_colorProgram;
    globjects::ref_ptr<globjects::Program>     m_normalProgram;
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;
    globjects::ref_ptr<globjects::Shader>      m_colorFragmentShader;
    globjects::ref_ptr<globjects::Shader>      m_normalFragmentShader;
};


} // namespace gloperate
