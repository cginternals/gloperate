
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>

namespace gloperate
{


class DemoAntialiasableTriangleStage;


/**
*  @brief
*    Demo pipeline displaying a rotating logo
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
    DemoMultiFramePipeline(Environment * environment, const std::string & name = "DemoMultiFramePipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoMultiFramePipeline();


protected:
    // Stages
    MultiFrameAggregationPipeline  * m_multiFramePipeline;
    DemoAntialiasableTriangleStage * m_frameRenderStage;
};


} // namespace gloperate
