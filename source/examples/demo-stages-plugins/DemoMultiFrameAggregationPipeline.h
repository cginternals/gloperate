
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate_glkernel {

class MultiFrameAggregationPipeline;

}


class TransparencyRenderingPipeline;


/**
*  @brief
*    Demo pipeline showing multiframe aggregation
*/
class DemoMultiFrameAggregationPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoMultiFrameAggregationPipeline, gloperate::Stage
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
    DemoMultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name = "DemoMultiFrameAggregationPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoMultiFrameAggregationPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::MultiFrameAggregationPipeline> m_multiFramePipeline;   ///< Aggregation Pipeline
    std::unique_ptr<TransparencyRenderingPipeline>                     m_transparencyPipeline; ///< Pipeline generating frames to aggregate
};
