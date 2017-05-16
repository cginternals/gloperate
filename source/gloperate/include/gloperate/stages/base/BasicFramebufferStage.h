
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


class RenderTarget;


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
    Input<glm::vec4>                 viewport;      ///< Texture size

    // Outputs
    Output<globjects::Framebuffer *>  fbo;          ///< Framebuffer
    Output<globjects::Texture *>      colorTexture; ///< Color texture
    Output<globjects::Texture *>      depthTexture; ///< Depth texture
    Output<gloperate::RenderTarget *> colorBuffer;  ///< Color attachment
    Output<gloperate::RenderTarget *> depthBuffer;  ///< Depth attachment


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
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void rebuildFBO();


protected:
    // Data
    std::unique_ptr<globjects::Framebuffer>  m_fbo;          ///< The created framebuffer
    std::unique_ptr<globjects::Texture>      m_colorTexture; ///< The created texture
    std::unique_ptr<globjects::Texture>      m_depthTexture; ///< The created texture
    std::unique_ptr<gloperate::RenderTarget> m_colorBuffer;  ///< Color texture
    std::unique_ptr<gloperate::RenderTarget> m_depthBuffer;  ///< Depth texture
};


} // namespace gloperate
