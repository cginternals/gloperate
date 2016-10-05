
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/GlmProperties.h>
#include <gloperate/pipeline/Stage.h>

#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace globjects {
class Texture;
class Framebuffer;
} // namespace globjects


namespace gloperate {
class DemoStage;
} // namespace gloperate


namespace gloperate_glheadless {


/**
*  @brief
*    Demo stage that renders a simple triangle into a texture using an offscreen context
*/
class GLOPERATE_GLHEADLESS_API DemoOffscreenStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoOffscreenStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that renders a simple triangle into a texture using an offscreen context"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.0.1"
    )


public:
    // Inputs
    gloperate::Input<glm::vec4> viewport;        ///< Texture size
    Input<glm::vec3>            backgroundColor; ///< Background color (RGB)
    Input<float>                timeDelta;       ///< Time delta since last frame (in seconds)

    // Outputs
    gloperate::Output<globjects::Texture *> colorTexture; ///< The rendered result


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
    explicit DemoOffscreenStage(gloperate::Environment * environment, const std::string & name = "DemoOffscreenStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoOffscreenStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void createAndSetUpOffscreenContext(gloperate::AbstractGLContext * context);
    void rebuildFBO();


protected:
    // Rendering objects
    std::unique_ptr<gloperate::AbstractGLContext> m_offscreenContext; ///< The offscreen context
    globjects::ref_ptr<globjects::Framebuffer>    m_framebuffer;      ///< The offscreen framebuffer
    globjects::ref_ptr<globjects::Texture>        m_colorTexture;     ///< Color texture
    globjects::ref_ptr<globjects::Texture>        m_depthTexture;     ///< Depth texture
    
    // Stages
    std::unique_ptr<gloperate::DemoStage> m_renderStage; ///< The stage executed in the offscreen context
};


} // namespace gloperate_glheadless
