
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
*
*  If a valid viewport is set (width and height are greater or equal to '0', only the area of
*  the given viewport is cleared, otherwise the full render targets are cleared.
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

    RenderInterface                  renderInterface;        ///< Renderinterface to manage render targets inputs and outputs


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


protected:
    std::vector<Input<glm::vec4> *> m_colorValueInputs;
    std::vector<Input<float> *> m_depthValueInputs;
    std::vector<Input<std::pair<float, int>> *> m_depthStencilValueInputs;
    std::unique_ptr<globjects::Framebuffer> m_defaultFBO; ///< Default FBO for clearing
    std::unique_ptr<globjects::Framebuffer> m_fbo;        ///< Intermediate FBO for clearing
};


} // namespace gloperate
