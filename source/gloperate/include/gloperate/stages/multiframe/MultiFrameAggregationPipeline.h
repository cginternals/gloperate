
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/stages/interfaces/MultiFrameRenderInterface.h>


namespace gloperate
{


class BasicFramebufferStage;
class CustomFramebufferStage;
class BlitStage;
class MultiFrameControlStage;
class MultiFrameAggregationStage;

class MultiFrameDiscDistributionStage;
class NoiseKernelStage;
class SSAOKernelStage;
class SubpixelAntialiasingOffsetStage;
class TransparencyKernelStage;


/**
*  @brief
*    Pipeline that aggregates multiple frames rendered by the given Stage/Pipeline
*/
class GLOPERATE_API MultiFrameAggregationPipeline : public Pipeline
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
    RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int> multiFrameCount; ///< Maximum number of frames to aggregate


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
    MultiFrameAggregationPipeline(Environment * environment, const std::string & name = "MultiFrameAggregationPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameAggregationPipeline();

    /**
    *  @brief
    *    Set the frame generating stage/pipeline
    *
    *  @param[in] interface
    *    Render interface of the frame generating stage
    */
    void setFrameRenderer(RenderInterface & interface);

    /**
    *  @brief
    *    Set the frame generating stage/pipeline
    *
    *  @param[in] interface
    *    Render interface of the frame generating stage
    */
    void setFrameRenderer(MultiFrameRenderInterface & interface);


protected:
    // Virtual Stage interface
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void connectBasicRenderInterface(RenderInterface & interface);
    void connectMultiFrameRenderInterface(MultiFrameRenderInterface & interface);
    void disconnectRenderStage();


protected:
    // Aggregation stages
    BasicFramebufferStage      * m_renderFramebufferStage;      ///< FBO stage for frame generating stage
    CustomFramebufferStage     * m_aggregationFramebufferStage; ///< Aggregation FBO
    MultiFrameControlStage     * m_controlStage;                ///< Multiframe control stage
    MultiFrameAggregationStage * m_aggregationStage;            ///< Aggregation stage
    BlitStage                  * m_blitStage;                   ///< Blit stage

    // Stages providing multiframe-specific data
    MultiFrameDiscDistributionStage * m_diskDistributionStage;  ///< Disk distrubution stage
    NoiseKernelStage                * m_noiseStage;             ///< Noise kernel stage
    SSAOKernelStage                 * m_ssaoStage;              ///< SSAO stage
    SubpixelAntialiasingOffsetStage * m_subpixelStage;          ///< Subpixel antialiasing offset stage
    TransparencyKernelStage         * m_transparencyStage;      ///< Transparency kernel stage

    // Inserted Stage/Pipeline
    Stage * m_frameRenderStage;                                 ///< Frame generating stage
};


} // namespace gloperate
