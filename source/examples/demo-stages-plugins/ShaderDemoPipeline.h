
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate
{
    class BasicFramebufferStage;
    class TextureLoadStage;
    class ShaderStage;
    class ProgramStage;
    class ClearStage;
    class RasterizationStage;
    class RenderPassStage;
    class BlitStage;
    class Quad;
}


/**
*  @brief
*    Demo pipeline displaying a static logo
*/
class ShaderDemoPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShaderDemoPipeline, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline displaying a static logo"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<cppassist::FilePath> shader1;         ///< Shader 1 filename
    Input<cppassist::FilePath> shader2;         ///< Shader 2 filename

    Input<cppassist::FilePath> texture;         ///< Texture filename


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
    ShaderDemoPipeline(gloperate::Environment * environment, const std::string & name = "ShaderDemoPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShaderDemoPipeline();

public:
    // Virtual Stage interface
    void onContextInit(gloperate::AbstractGLContext * context) override;
    void onContextDeinit(gloperate::AbstractGLContext * context) override;

protected:
    // Stages
    std::unique_ptr<gloperate::TextureLoadStage>      m_textureLoadStage;   ///< Stage that loads a static picture

    std::unique_ptr<gloperate::ShaderStage>           m_shaderStage;        ///< Stage which loads one shader

    std::unique_ptr<gloperate::ProgramStage>          m_programStage;       ///< Stage which creates the program

    std::unique_ptr<gloperate::RenderPassStage>       m_renderPassStage;    ///< Stage which creates the render pass
    std::unique_ptr<gloperate::RasterizationStage>    m_rasterizationStage; ///< Stage which renders the scene

    // Geometry
    std::unique_ptr<gloperate::Quad> m_quad;
};
