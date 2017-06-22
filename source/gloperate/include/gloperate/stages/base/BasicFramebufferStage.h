
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


class ColorRenderTarget;
class DepthRenderTarget;
class StencilRenderTarget;


/**
*  @brief
*    Stage that maintains a basic framebuffer with a color and depth attachment
*/
class GLOPERATE_API BasicFramebufferStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        BasicFramebufferStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that maintains a basic framebuffer with a color and depth attachment"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<glm::vec4>                       viewport;     ///< Texture size

    // Outputs
    Output<globjects::Texture *>             colorTexture;        ///< Color texture
    Output<globjects::Texture *>             depthStencilTexture; ///< Combined depth-stencil texture
    Output<gloperate::ColorRenderTarget *>   colorBuffer;         ///< Color attachment
    Output<gloperate::DepthRenderTarget *>   depthBuffer;         ///< Depth attachment
    Output<gloperate::StencilRenderTarget *> stencilBuffer;       ///< Stencil attachment


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
    BasicFramebufferStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~BasicFramebufferStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<globjects::Texture>             m_colorTexture;        ///< Internal color texture
    std::unique_ptr<globjects::Texture>             m_depthStencilTexture; ///< Internal, combined depth-stencil texture
    std::unique_ptr<gloperate::ColorRenderTarget>   m_colorBuffer;         ///< Color texture
    std::unique_ptr<gloperate::DepthRenderTarget>   m_depthBuffer;         ///< Depth texture
    std::unique_ptr<gloperate::StencilRenderTarget> m_stencilBuffer;       ///< Stencil texture
};


} // namespace gloperate
