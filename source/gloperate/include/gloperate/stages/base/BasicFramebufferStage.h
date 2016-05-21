
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>

#include <gloperate/base/GlmProperties.h>
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
    CPPEXPOSE_DECLARE_COMPONENT(BasicFramebufferStage, gloperate::Stage)


public:
    // Input
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
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    BasicFramebufferStage(ViewerContext * viewerContext, const std::string & name = "BasicFramebufferStage", Pipeline * parent = nullptr);

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
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;       ///< The created framebuffer
    globjects::ref_ptr<globjects::Texture>     m_texColor;  ///< Color texture
    globjects::ref_ptr<globjects::Texture>     m_texDepth;  ///< Depth texture
};


} // namespace gloperate
