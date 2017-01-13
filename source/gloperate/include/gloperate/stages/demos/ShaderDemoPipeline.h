
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class BasicFramebufferStage;
class TextureLoadStage;
class ShaderStage;
class ProgramStage;
class DemoDrawableStage;
class MixerStage;
class RasterizationStage;
class RenderPassStage;


/**
*  @brief
*    Demo pipeline displaying a static logo
*/
class GLOPERATE_API ShaderDemoPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ShaderDemoPipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline displaying a static logo"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<cppassist::FilePath> shader1; ///< Shader 1 filename
    Input<cppassist::FilePath> shader2; ///< Shader 2 filename

    Input<cppassist::FilePath> texture; ///< Texture filename

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
    ShaderDemoPipeline(Environment * environment, const std::string & name = "ShaderDemoPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ShaderDemoPipeline();

protected:
    // Stages
    TextureLoadStage      * m_textureLoadStage;  ///< Stage that loads a static picture

    ShaderStage           * m_shaderStage;      ///< Stage which loads one shader

    ProgramStage          * m_programStage;     ///< Stage which creates the program

    BasicFramebufferStage * m_framebufferStage;  ///< Stage which creates the framebuffer
    DemoDrawableStage     * m_demoDrawableStage; ///< Stage which creates the drawable

    RenderPassStage *       m_renderPassStage;   ///< Stage which creates the render pass
    RasterizationStage *    m_rasterizationStage;///< Stage which renders the scene

    MixerStage            * m_mixerStage;        ///< Stage that renders the output to the screen
};


} // namespace gloperate
