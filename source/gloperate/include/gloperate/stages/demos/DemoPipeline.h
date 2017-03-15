
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
class FramebufferStage;
class TextureLoadStage;
class MixerStage;
class TimerStage;
class SpinningRectStage;
class TextureStage;
class ProgramStage;
class RenderPassStage;
class RasterizationStage;
class ScreenAlignedQuad;


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
    Input<cppassist::FilePath> texture; ///< Texture filename
    Input<float>               angle;   ///< Current rotation angle
    Input<bool>                rotate;  ///< Rotation automatically?
    Input<Color>               color;   ///< Mixer color

    Input<cppassist::FilePath> shader1; ///< Shader 1 filename
    Input<cppassist::FilePath> shader2; ///< Shader 2 filename


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
    DemoPipeline(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoPipeline();


protected:
    void onRotateChanged(const bool & rotate);
    void onContextInit(AbstractGLContext * context);


protected:
    // Stages
    std::unique_ptr<TextureLoadStage> m_textureLoadStage;               ///< Stage that loads a static picture
    std::unique_ptr<TimerStage> m_timerStage;                           ///< Timer for continuous rendering and animation

    std::unique_ptr<BasicFramebufferStage> m_framebufferStage1;         ///< Framebuffer for rendering the spinning rect
    std::unique_ptr<SpinningRectStage> m_spinningRectStage;             ///< Stage that renders the spinning rect

    std::unique_ptr<BasicFramebufferStage> m_framebufferStage2;         ///< Framebuffer for rendering the colorized output
    std::unique_ptr<ProgramStage> m_colorizeProgramStage;               ///< Builds the Program for blending an image with a color
    std::unique_ptr<RenderPassStage> m_colorizeRenderPassStage;         ///< Builds the RenderPass for the same task
    std::unique_ptr<RasterizationStage> m_colorizeRasterizationStage;   ///< Executes this RenderPass on the inputs

    std::unique_ptr<MixerStage> m_mixerStage;                           ///< Stage that renders the output to the screen

protected:
    // Members
    std::unique_ptr<ScreenAlignedQuad> m_screenAlignedQuad; ///< Simple Screen Aligned Quad for Colorization in Demo
};


} // namespace gloperate
