
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

#include <demo-stages/demo-stages_api.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that renders a rotating rectangle onto the screen
*
*  @remarks
*    This stage is part of the DemoPipeline
*/
class DEMO_STAGES_API DemoRenderStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoRenderStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that renders a rotating rectangle onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface                  renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Texture *>      texture;         ///< Texture object
    Input<float>                     angle;           ///< Current angle of rotating triangle (in radians)
    Input<globjects::Texture *>      colorTexture;    ///< Target color texture
    Input<globjects::Program *>      program;         ///< Program object

    // Outputs
    Output<globjects::Framebuffer *> fboOut;          ///< Pass through of target FBO
    Output<globjects::Texture *>     colorTextureOut; ///< Pass through of target color texture


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
    DemoRenderStage(Environment * environment, const std::string & name = "RenderStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoRenderStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void setupGeometry();
    void setupCamera();


protected:
    // Rendering objects
    gloperate::Camera                          m_camera;
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;
};


} // namespace gloperate
