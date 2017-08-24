
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec4.hpp>

#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>

#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate/rendering/ColorRenderTarget.h>


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
    Input<gloperate::ColorRenderTarget *> source;         ///< Source render target
    Input<glm::vec4>                      sourceViewport; ///< Source Viewport for reading from source
    Input<gloperate::ColorRenderTarget *> target;         ///< Target render target
    Input<glm::vec4>                      targetViewport; ///< Target viewport for writing into target
    Input<gl::GLenum>                     minFilter;      ///< Interpolation mode used when target size is lower than source size (default: linear interpolation)
    Input<gl::GLenum>                     magFilter;      ///< Interpolation mode used when target size is greater than source size (default: nearest filtering)

    // Outputs
    Output<gloperate::ColorRenderTarget *> targetOut;     ///< Pass-through target render target


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

    /**
    *  @brief
    *    Virtual destructor
    */
    virtual ~BlitStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;


protected:
    std::unique_ptr<globjects::Framebuffer>       m_defaultFBO;         ///< Intermediate default FBO
    std::unique_ptr<globjects::Framebuffer>       m_sourceFBO;          ///< Intermediate source FBO
    std::unique_ptr<globjects::Framebuffer>       m_targetFBO;          ///< Intermediate target FBO
    std::unique_ptr<globjects::Framebuffer>       m_intermediateFBO;    ///< Intermediate FBO for same-target blitting
    std::unique_ptr<gloperate::ColorRenderTarget> m_intermediateTarget; ///< Intermediate render target for same-target blitting
    std::unique_ptr<globjects::Renderbuffer>      m_intermediateBuffer; ///< Intermediate render buffer for same-target blitting
};


} // namespace gloperate
