
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/GlmProperties.h>
#include <gloperate/base/ExtraProperties.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class BasicFramebufferStage;
class TextureLoadStage;
class MixerStage;
class TimerStage;
class SpinningRectStage;
class ColorizeStage;


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
    Input<glm::vec4>           color;   ///< Mixer color


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
    void onRotateChanged(const bool & rotate);


protected:
    // Stages
    TextureLoadStage       * m_textureLoadStage;    ///< Stage that loads a static picture
    TimerStage             * m_timerStage;          ///< Timer for continuous rendering and animation

    BasicFramebufferStage  * m_framebufferStage1;   ///< Framebuffer for rendering the spinning rect
    SpinningRectStage      * m_spinningRectStage;   ///< Stage that renders the spinning rect

    BasicFramebufferStage  * m_framebufferStage2;   ///< Framebuffer for rendering the colorized output
    ColorizeStage          * m_colorizeStage;       ///< Stage that blends the image with a color

    MixerStage             * m_mixerStage;          ///< Stage that renders the output to the screen
};


} // namespace gloperate
