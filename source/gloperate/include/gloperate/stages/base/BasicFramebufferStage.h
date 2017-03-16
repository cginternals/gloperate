
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


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
    Output<globjects::Framebuffer *> fbo;           ///< Framebuffer
    Output<globjects::Texture *>     colorTexture;  ///< Color attachment
    Output<globjects::Texture *>     depthTexture;  ///< Depth attachment


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
    std::unique_ptr<globjects::Framebuffer> m_fbo;       ///< The created framebuffer
    std::unique_ptr<globjects::Texture>     m_texColor;  ///< Color texture
    std::unique_ptr<globjects::Texture>     m_texDepth;  ///< Depth texture
};


} // namespace gloperate
