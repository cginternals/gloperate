
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
    class TextureStage;
    class ProgramStage;
    class RenderPassStage;
    class RasterizationStage;
    class BlitStage;
    class ScreenAlignedQuad;
    class Quad;
}

class SpinningRectStage;
class TimerStage;


/**
*  @brief
*    Demo pipeline displaying a rotating logo
*/
class DemoPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoPipeline, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline displaying a rotating logo"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<cppassist::FilePath> texture;         ///< Texture filename
    Input<float>               angle;           ///< Current rotation angle
    Input<bool>                rotate;          ///< Rotation automatically?
    Input<gloperate::Color>    color;           ///< Mixer color

    Input<cppassist::FilePath> shader1;         ///< Shader 1 filename
    Input<cppassist::FilePath> shader2;         ///< Shader 2 filename


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
    DemoPipeline(gloperate::Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoPipeline();


protected:
    void onRotateChanged(const bool & rotate);
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;


protected:
    // Stages
    std::unique_ptr<gloperate::TextureLoadStage>      m_textureLoadStage;           ///< Stage that loads a static picture
    std::unique_ptr<TimerStage>                       m_timerStage;                 ///< Timer for continuous rendering and animation

    std::unique_ptr<gloperate::BasicFramebufferStage> m_framebufferStage1;          ///< Framebuffer for rendering the spinning rect
    std::unique_ptr<SpinningRectStage>                m_spinningRectStage;          ///< Stage that renders the spinning rect

    std::unique_ptr<gloperate::BasicFramebufferStage> m_framebufferStage2;          ///< Framebuffer for rendering the colorized output
    std::unique_ptr<gloperate::ProgramStage>          m_colorizeProgramStage;       ///< Builds the Program for blending an image with a color
    std::unique_ptr<gloperate::RenderPassStage>       m_colorizeRenderPassStage;    ///< Builds the RenderPass for the same task
    std::unique_ptr<gloperate::RasterizationStage>    m_colorizeRasterizationStage; ///< Executes this RenderPass on the inputs

    std::unique_ptr<gloperate::BlitStage>             m_blitStage;                  ///< Stage that renders the output to the screen

    // Internal data
    std::unique_ptr<gloperate::Quad>                  m_quad;                       ///< Screen-aligned quad for colorization in demo
};
