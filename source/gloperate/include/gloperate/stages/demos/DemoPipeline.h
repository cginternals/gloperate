
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
class ShaderLoaderStage;
class ShaderStage;
class ProgramStage;
class DemoRenderStage;
class MixerStage;


/**
*  @brief
*    Demo pipeline displaying a rotating logo
*/
class GLOPERATE_API DemoPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoPipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline displaying a rotating logo"
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
    DemoPipeline(Environment * environment, const std::string & name = "DemoPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoPipeline();

protected:
    // Stages
    TextureLoadStage      * m_textureLoadStage; ///< Stage that loads a static picture

    ShaderLoaderStage     * m_shaderLoadStage1; ///< Stages that loads shader sources code from a file
    ShaderStage           * m_shaderStage1;     ///< Stage which creates the shader

    ShaderLoaderStage     * m_shaderLoadStage2; ///< Stages that loads shader sources code from a file
    ShaderStage           * m_shaderStage2;     ///< Stage which creates the shader

    ProgramStage          * m_programStage;     ///< Stage which creates the program

    BasicFramebufferStage * m_framebufferStage; ///< Stage which creates the framebuffer
    DemoRenderStage           * m_renderStage;      ///< Stage that renders with the program

    MixerStage            * m_mixerStage;       ///< Stage that renders the output to the screen
};


} // namespace gloperate
