
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec4.hpp>

#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate/rendering/RenderTarget.h>


namespace gloperate
{


/**
*  @brief
*    Stage for blitting from one framebuffer into another
*/
class GLOPERATE_API BlitStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        BlitStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage for blitting from one framebuffer into another"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<gloperate::RenderTarget *> source;         ///< FBO containing the source attachments
    Input<glm::vec4>                 sourceViewport; ///< Viewport for reading from source FBO
    Input<gloperate::RenderTarget *> target;         ///< FBO with destination attachments
    Input<glm::vec4>                 targetViewport; ///< Viewport for writing into destination FBO
    Input<gl::GLenum>                minFilter;      ///< Interpolation mode used when target size is lower than source size (default: linear interpolation)
    Input<gl::GLenum>                magFilter;      ///< Interpolation mode used when target size is greater than source size (default: nearest filtering)

    // Outputs
    Output<gloperate::RenderTarget *> targetOut;     ///< Pass-through render target


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
    BlitStage(Environment * environment, const std::string & name = "BlitStage");


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;


protected:
    std::unique_ptr<globjects::Framebuffer> m_defaultFBO; ///< Intermediate default FBO
    std::unique_ptr<globjects::Framebuffer> m_sourceFBO; ///< Intermediate source FBO
    std::unique_ptr<globjects::Framebuffer> m_targetFBO; ///< Intermediate target FBO
};


} // namespace gloperate
