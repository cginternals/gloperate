
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class Framebuffer;
    class FramebufferAttachment;
}


namespace gloperate
{


/**
*  @brief
*    Stage that clears the screen with a background color
*/
class GLOPERATE_API ClearStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ClearStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that clears the screen with a background color"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs

    Input<bool>                      clear;                  ///< Flag if buffers should get cleared

    Input<glm::vec4>                 viewport;               ///< Region for clearing

    Input<gloperate::RenderTarget *> colorAttachment0;       ///< Color attachment 0 to clear
    Input<glm::vec4>                 clearColor0;            ///< Clear color for attachment 0

    Input<gloperate::RenderTarget *> colorAttachment1;       ///< Color attachment 1 to clear
    Input<glm::vec4>                 clearColor1;            ///< Clear color for attachment 1

    Input<gloperate::RenderTarget *> colorAttachment2;       ///< Color attachment 2 to clear
    Input<glm::vec4>                 clearColor2;            ///< Clear color for attachment 2

    Input<gloperate::RenderTarget *> colorAttachment3;       ///< Color attachment 3 to clear
    Input<glm::vec4>                 clearColor3;            ///< Clear color for attachment 3

    Input<gloperate::RenderTarget *> colorAttachment4;       ///< Color attachment 4 to clear
    Input<glm::vec4>                 clearColor4;            ///< Clear color for attachment 4

    // [TODO]: add until index 15

    Input<gloperate::RenderTarget *> depthAttachment;        ///< Depth only attachment to clear
    Input<float>                     depthValue;             ///< Clear depth for both depth attachments

    Input<gloperate::RenderTarget *> depthStencilAttachment; ///< Combined depth stencil attachment to clear
    Input<int>                       stencilValue;           ///< Clear stencil for combined depth stencil attachment

    // Outputs

    Output<gloperate::RenderTarget *> colorAttachment0Out;       ///< Pass-through color attachment 0
    Output<gloperate::RenderTarget *> colorAttachment1Out;       ///< Pass-through color attachment 1
    Output<gloperate::RenderTarget *> colorAttachment2Out;       ///< Pass-through color attachment 2
    Output<gloperate::RenderTarget *> colorAttachment3Out;       ///< Pass-through color attachment 3
    Output<gloperate::RenderTarget *> colorAttachment4Out;       ///< Pass-through color attachment 4
    // [TODO]: add until index 15
    Output<gloperate::RenderTarget *> depthAttachmentOut;        ///< Pass-through depth only attachment
    Output<gloperate::RenderTarget *> depthStencilAttachmentOut; ///< Pass-through depth stencil attachment


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
    ClearStage(Environment * environment, const std::string & name = "ClearStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ClearStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;
    virtual void onContextInit(AbstractGLContext * content) override;
    virtual void onContextDeinit(AbstractGLContext * content) override;

    void clearColorAttachment(unsigned char index, gloperate::RenderTarget * target, const glm::vec4 & clearColor);
    void clearDepthAttachment(gloperate::RenderTarget * target, float clearDepth);
    void clearDepthStencilAttachment(gloperate::RenderTarget * target, float clearDepth, int clearStencil);


protected:
    std::unique_ptr<globjects::Framebuffer> m_defaultFBO; ///< Default FBO for clearing
    std::unique_ptr<globjects::Framebuffer> m_fbo;        ///< Intermediate FBO for clearing
};


} // namespace gloperate
