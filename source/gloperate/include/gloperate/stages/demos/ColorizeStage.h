
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/rendering/ScreenAlignedQuad.h>


namespace gloperate
{


/**
*  @brief
*    Demo stage that applies a color to a texture
*/
class GLOPERATE_API ColorizeStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ColorizeStage, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo stage that applies a color to a texture"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface                  renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<globjects::Texture *>      texture;         ///< Texture object
    Input<globjects::Texture *>      colorTexture;    ///< Target color texture
    Input<Color>                     color;           ///< Color to mix with

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
    ColorizeStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ColorizeStage();


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
    gloperate::Camera                               m_camera;
    std::unique_ptr<gloperate::ScreenAlignedQuad>   m_screenAlignedQuad;
    std::unique_ptr<globjects::VertexArray>         m_vao;
    std::unique_ptr<globjects::Buffer>              m_vertexBuffer;
    std::unique_ptr<globjects::Program>             m_program;

    std::unique_ptr<globjects::AbstractStringSource> m_vSource;
    std::unique_ptr<globjects::AbstractStringSource> m_fSource;
    std::unique_ptr<globjects::Shader>               m_vertexShader;
    std::unique_ptr<globjects::Shader>               m_fragmentShader;
};


} // namespace gloperate
