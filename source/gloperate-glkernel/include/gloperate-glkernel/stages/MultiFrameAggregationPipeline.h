
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>
#include <gloperate-glkernel/stages/MultiFrameControlStage.h>


namespace gloperate
{
    class BasicFramebufferStage;
    class TextureRenderTargetStage;
    class RenderbufferRenderTargetStage;
    class FramebufferStage;
    class BlitStage;
} // namespace gloperate


namespace gloperate_glkernel
{


class MultiFrameAggregationStage;
class IntermediateFramePreparationStage;


/**
*  @brief
*    Pipeline that aggregates multiple frames rendered by the given Stage/Pipeline
*/
class GLOPERATE_GLKERNEL_API MultiFrameAggregationPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameAggregationPipeline, gloperate::Stage
      , ""
      , ""
      , ""
      , "Pipeline that aggregates multiple frames rendered by the given Stage/Pipeline"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface            canvasInterface;   ///< Interface for rendering into a viewer

    // Inputs
    Input<int>                            multiFrameCount;   ///< Maximum number of frames to aggregate
    Input<gloperate::ColorRenderTarget*>  aggregationTarget; ///< RenderTarget to aggregate into

    // Outputs
    Output<gloperate::ColorRenderTarget*> aggregatedTarget;  ///< RenderTarget with aggregated content


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
    MultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name = "MultiFrameAggregationPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameAggregationPipeline();


public:
    /**
    *  @brief
    *    Let a slot cause the aggregation to restart, when it's value changes.
    */
    template <typename T>
    gloperate::Input<T> * restartAggregationOn(gloperate::Slot<T> * slot);


protected:
    /**
    *  @brief
    *    Set the intermediate frame generating stage/pipeline
    *
    *  @param[in] stage
    *    The render stage
    */
    void setRenderStage(gloperate::Stage * stage);

    /**
    *  @brief
    *    Disconnects the current render stage.
    */
    void disconnectRenderStage();


protected:
    // Aggregation stages
    std::unique_ptr<gloperate::TextureRenderTargetStage>      m_colorRenderTargetStage;        ///< Render target for frame rendering
    std::unique_ptr<gloperate::TextureRenderTargetStage>      m_aggregationRenderTargetStage;  ///< Render target the aggregation happens on
    std::unique_ptr<gloperate::RenderbufferRenderTargetStage> m_depthStencilRenderTargetStage; ///< Aggregation depth stencil render target
    std::unique_ptr<MultiFrameControlStage>                   m_controlStage;                  ///< Multiframe control stage
    std::unique_ptr<IntermediateFramePreparationStage>        m_framePreparationStage;         ///< Intermediate frame preparation stage
    std::unique_ptr<MultiFrameAggregationStage>               m_aggregationStage;              ///< Aggregation stage
    std::unique_ptr<gloperate::BlitStage>                     m_blitStage;                     ///< Blit from aggregation to output

    // Inserted Stage/Pipeline
    Stage                                                   * m_renderStage;                   ///< Actual rendering stage, providing intermediate frames
};


} // namespace gloperate_glkernel


#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.inl>
