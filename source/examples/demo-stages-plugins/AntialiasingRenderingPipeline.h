
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate_glkernel
{
    class DiscDistributionKernelStage;
}

namespace gloperate
{
    class SubpixelAntialiasingOffsetStage;
}


class AntialiasableTriangleStage;


/**
*  @brief
*    Demo pipeline that renders a static triangle onto the screen
*/
class AntialiasingRenderingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        AntialiasingRenderingPipeline, gloperate::Stage
      , ""                              // Tags
      , ""                              // Icon
      , ""                              // Annotations
      , "Demo pipeline that renders a static triangle onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int> multiFrameCount;                 ///< Total number of frames to aggregate


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
    AntialiasingRenderingPipeline(gloperate::Environment * environment, const std::string & name = "AntialiasingRenderingPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AntialiasingRenderingPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::DiscDistributionKernelStage> m_subpixelStage;  ///< Stage generating subpixel offset for antialiasing
    std::unique_ptr<AntialiasableTriangleStage>                      m_triangleStage;  ///< Rendering stage
};
