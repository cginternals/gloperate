
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate
{
    class TrackballStage;
}


namespace gloperate_glkernel
{
    class MultiFrameAggregationPipeline;
}


class MultiFrameRenderingPipeline;


/**
*  @brief
*    Demo pipeline showing multiframe aggregation
*/
class DemoMultiFrameEffectsPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoMultiFrameEffectsPipeline, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline showing multiframe aggregation"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int>                 multiFrameCount; ///< Number of frames to aggregate


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the pipeline belongs (must NOT be null!)
    *  @param[in] name
    *    Pipeline name
    */
    DemoMultiFrameEffectsPipeline(gloperate::Environment * environment, const std::string & name = "DemoMultiFrameEffectsPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoMultiFrameEffectsPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::MultiFrameAggregationPipeline> m_multiFramePipeline; ///< Aggregation Pipeline
    std::unique_ptr<MultiFrameRenderingPipeline>                       m_renderingPipeline;  ///< Demo pipeline generating frames to aggregate
    std::unique_ptr<gloperate::TrackballStage>                         m_trackballStage;     ///< Stage for trackball navigation
};
