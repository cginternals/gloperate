
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


namespace gloperate
{

class BasicFramebufferStage;
class CustomFramebufferStage;
class BlitStage;

}


namespace gloperate_glkernel
{


class MultiFrameControlStage;
class MultiFrameAggregationStage;


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
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

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
    MultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name = "MultiFrameAggregationPipeline");

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
    void setFrameRenderer(gloperate::RenderInterface & interface);


protected:
    // Virtual Stage interface
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

    // Helper functions
    void connectBasicRenderInterface(gloperate::RenderInterface & interface);
    void disconnectRenderStage();


protected:
    // Aggregation stages
    std::unique_ptr<gloperate::BasicFramebufferStage>  m_renderFramebufferStage;      ///< FBO stage for frame generating stage
    std::unique_ptr<gloperate::CustomFramebufferStage> m_aggregationFramebufferStage; ///< Aggregation FBO
    std::unique_ptr<MultiFrameControlStage>            m_controlStage;                ///< Multiframe control stage
    std::unique_ptr<MultiFrameAggregationStage>        m_aggregationStage;            ///< Aggregation stage
    std::unique_ptr<gloperate::BlitStage>              m_blitStage;                   ///< Blit stage

    // Inserted Stage/Pipeline
    Stage * m_frameRenderStage;                                 ///< Frame generating stage
};


} // namespace gloperate
