
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


namespace gloperate_glkernel
{


/**
*  @brief
*    Stage that ensures the intermediate frame is stored in a texture for further aggregation
*/
class GLOPERATE_GLKERNEL_API IntermediateFramePreparationStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        IntermediateFramePreparationStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that ensures the intermediate frame is in the output color attachment"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Render Interface
    gloperate::RenderInterface            renderInterface;             ///< Render interface for aggregation target

    // Inputs
    Input<gloperate::ColorRenderTarget *> intermediateRenderTarget;    ///< Intermediate frame render target
    Input<globjects::Texture           *> intermediateFrameTexture;    ///< Designated intermediate frame texture

    // Outputs
    Output<globjects::Texture          *> intermediateFrameTextureOut; ///< Intermediate frame texture with same contents as intermediateRenderTarget


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
    IntermediateFramePreparationStage(gloperate::Environment * environment, const std::string & name = "IntermediateFramePreparationStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~IntermediateFramePreparationStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    // Data
    std::unique_ptr<globjects::Framebuffer> m_targetFBO;  ///< Framebuffer for blitting target
};


} // namespace gloperate_glkernel
