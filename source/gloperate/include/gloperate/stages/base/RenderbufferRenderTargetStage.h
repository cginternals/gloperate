
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Renderbuffer;
}


namespace gloperate
{


class RenderTarget;


/**
*  @brief
*    Stage that creates an empty texture with a specified size and format
*/
class GLOPERATE_API RenderbufferRenderTargetStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        RenderbufferRenderTargetStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates an empty renderbuffer as render target with a specified size and format"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<gl::GLenum> internalFormat;     ///< OpenGL internal image format
    Input<glm::vec4>  size;               ///< Viewport size

    // Outputs
    Output<globjects::Renderbuffer *> renderbuffer; ///< Texture
    Output<gloperate::RenderTarget *> renderTarget; ///< RenderTarget


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
    RenderbufferRenderTargetStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderbufferRenderTargetStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<globjects::Renderbuffer> m_renderbuffer; ///< The created renderbuffer
    std::unique_ptr<gloperate::RenderTarget> m_renderTarget; ///< The passed render target
};


} // namespace gloperate
