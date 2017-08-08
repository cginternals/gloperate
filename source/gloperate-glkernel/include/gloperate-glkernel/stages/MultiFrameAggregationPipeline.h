
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


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


class MultiFrameControlStage;
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
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int>                 multiFrameCount; ///< Maximum number of frames to aggregate


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

    /**
    *  @brief
    *    Add non-default input to the pipeline
    *
    *  @param[in] input
    *    The input to be added
    */
    void addThroughputInput(gloperate::AbstractSlot * input);

    /**
    *  @brief
    *    Connect a non-default input to the rendering stage/pipeline
    *
    *  @param[in] input
    *    The input to be connected
    */
    void connectThroughputInput(gloperate::AbstractSlot * input);

    /**
    *  @brief
    *    Remove non-default input from the pipeline
    *
    *  @param[in] input
    *    The input to be removed
    */
    void removeThroughputInput(gloperate::AbstractSlot * input);


protected:
    // Aggregation stages
    std::unique_ptr<gloperate::TextureRenderTargetStage>      m_colorRenderTargetStage;        ///< Aggregation color render target
    std::unique_ptr<gloperate::RenderbufferRenderTargetStage> m_depthStencilRenderTargetStage; ///< Aggregation depth stencil render target
    std::unique_ptr<MultiFrameControlStage>                   m_controlStage;                  ///< Multiframe control stage
    std::unique_ptr<IntermediateFramePreparationStage>        m_framePreparationStage;         ///< Intermediate frame preparation stage
    std::unique_ptr<MultiFrameAggregationStage>               m_aggregationStage;              ///< Aggregation stage

    // Inserted Stage/Pipeline
    Stage                                                   * m_renderStage;                   ///< Actual rendering stage, providing intermediate frames

    // Input troughput
    std::set<gloperate::AbstractSlot *>                       m_additionalInputs;              ///< Registry of non-default inputs
};


} // namespace gloperate_glkernel
