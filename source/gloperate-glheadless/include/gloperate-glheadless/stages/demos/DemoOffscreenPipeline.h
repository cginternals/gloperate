
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/GlmProperties.h>

#include <gloperate-glheadless/stages/OffscreenPipeline.h>


namespace globjects {
class Texture;
} // namespace globjects


namespace gloperate {
class BasicFramebufferStage;
class DemoStage;
} // namespace gloperate


namespace gloperate_glheadless {


/**
*  @brief
*    Demo pipeline rendering a rotating logo to an offscreen context
*/
class GLOPERATE_GLHEADLESS_API DemoOffscreenPipeline : public OffscreenPipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoOffscreenPipeline, gloperate::Stage
        , "RenderStage"   // Tags
        , ""              // Icon
        , ""              // Annotations
        , "Demo pipeline displaying a rotating logo"
        , GLOPERATE_AUTHOR_ORGANIZATION
        , "v1.0.0"
    )


public:
    // Inputs
    gloperate::Input<glm::vec4> viewport;        ///< Texture size
    Input<glm::vec3>            backgroundColor; ///< Background color (RGB)
    Input<float>                timeDelta;       ///< Time delta since last frame (in seconds)

    // Outputs
    gloperate::Output<globjects::Texture *> colorTexture; ///< The rendered result
    Output<bool>                            rendered;     ///< 'true' if output has been rendered


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
    explicit DemoOffscreenPipeline(gloperate::Environment * environment, const std::string & name = "DemoOffscreenPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoOffscreenPipeline();


protected:
    // Stages
    gloperate::BasicFramebufferStage * m_framebufferStage;
    gloperate::DemoStage             * m_renderStage;
};


} // namespace gloperate_glheadless
