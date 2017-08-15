
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glbinding/gl/types.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


class AbstractRenderTarget;
class ColorRenderTarget;
class DepthRenderTarget;
class StencilRenderTarget;
class DepthStencilRenderTarget;


/**
*  @brief
*    Stage that extracts the texture from a RenderTarget, if one is attached.
*/
class GLOPERATE_API TextureFromRenderTargetExtractionStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TextureFromRenderTargetExtractionStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that extracts the texture from a RenderTarget, if one is attached"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<gloperate::ColorRenderTarget        *> colorRenderTarget;        ///< Color render target
    Input<gloperate::DepthRenderTarget        *> depthRenderTarget;        ///< Depth render target
    Input<gloperate::StencilRenderTarget      *> stencilRenderTarget;      ///< Stencil render target
    Input<gloperate::DepthStencilRenderTarget *> depthStencilRenderTarget; ///< Depth stencil render target

    // Outputs
    Output<globjects::Texture *>          texture;           ///< Internal texture of render target


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
    TextureFromRenderTargetExtractionStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureFromRenderTargetExtractionStage();


protected:
    // Virtual Stage interface
    virtual void onProcess() override;

    /**
    *  @brief
    *    Extract texture from RenderTarget and update output
    */
    void extractTexture(AbstractRenderTarget * renderTarget);
};


} // namespace gloperate
