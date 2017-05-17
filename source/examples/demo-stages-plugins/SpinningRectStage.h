
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
#include <gloperate/rendering/Camera.h>


/**
*  @brief
*    Demo stage that renders a rotating rectangle onto the screen
*
*  @remarks
*    This stage is part of the DemoPipeline
*/
class SpinningRectStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        SpinningRectStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that renders a rotating rectangle onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface       renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Texture *>      texture;         ///< Texture object
    Input<float>                     angle;           ///< Current angle of rotating triangle (in radians)
    Input<globjects::Texture *>      colorTexture;    ///< Target color texture

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
    SpinningRectStage(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~SpinningRectStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void setupGeometry();
    void setupCamera();
    void setupProgram();


protected:
    // Rendering objects
    gloperate::Camera                           m_camera;
    std::unique_ptr<globjects::VertexArray>     m_vao;
    std::unique_ptr<globjects::Buffer>          m_vertexBuffer;
    std::unique_ptr<globjects::Program>         m_program;
    std::unique_ptr<globjects::Shader>          m_vertexShader;
    std::unique_ptr<globjects::Shader>          m_fragmentShader;
};
