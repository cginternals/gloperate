
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


class RenderTarget;
class Texture;


/**
*  @brief
*    Stage that maintains a framebuffer attached with all input textures
*
*    By default it only has one color texture and one depth texture input.
*    Additional attachments (of type RenderTarget *) can be added as inputs dynamically.
*    These will be added as color attachments in order of addition.
*/
class GLOPERATE_API FramebufferStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        FramebufferStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that maintains a framebuffer attached with all input textures"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<RenderTarget *> colorTexture;  ///< Color attachment (#0)
    Input<RenderTarget *> depthTexture;  ///< Depth attachment

    // Additional attachments (of type RenderTarget *) can be added as inputs dynamically

    // Outputs
    Output<globjects::Framebuffer *> fbo;      ///< Framebuffer


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
    FramebufferStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~FramebufferStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void rebuildFBO();
    bool isNameOfDepthRenderTarget(const std::string & name);


protected:
    // Data
    globjects::ref_ptr<globjects::Framebuffer> m_fbo; ///< The created framebuffer
};


} // namespace gloperate
