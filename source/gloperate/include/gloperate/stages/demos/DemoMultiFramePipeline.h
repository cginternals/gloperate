
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class MultiFrameAggregationPipeline;
class DemoAntialiasingPipeline;
class DemoDOFPipeline;
class DemoTransparencyPipeline;
class DemoSSAOPipeline;


/**
*  @brief
*    Demo pipeline showing multiframe aggregation
*/
class GLOPERATE_API DemoMultiFramePipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoMultiFramePipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline showing multiframe aggregation"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int> multiFrameCount;      ///< Number of frames to aggregate


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
    DemoMultiFramePipeline(Environment * environment, const std::string & name = "DemoMultiFramePipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoMultiFramePipeline();


protected:
    // Stages
    MultiFrameAggregationPipeline * m_multiFramePipeline;   ///< Aggregation Pipeline

    // Demo pipelines using multiframe effects
    DemoAntialiasingPipeline      * m_antialiasingPipeline;
    DemoDOFPipeline               * m_dofPipeline;
    DemoTransparencyPipeline      * m_transparencyPipeline;
    DemoSSAOPipeline              * m_ssaoPipeline;
};


} // namespace gloperate
