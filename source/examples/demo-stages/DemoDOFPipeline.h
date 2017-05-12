
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate_glkernel {

class DiscDistributionKernelStage;

}

namespace gloperate
{

class MultiFrameDiscDistributionStage;

}

class DemoDOFCubeStage;


/**
*  @brief
*    Demo pipeline that renders a static cube onto the screen
*/
class DemoDOFPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoDOFPipeline, gloperate::Stage
      , "RenderStage Demo DOF" // Tags
      , ""                     // Icon
      , ""                     // Annotations
      , "Demo pipeline that renders a static cube onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int> multiFrameCount;      ///< Total number of multiframes to render


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
    DemoDOFPipeline(gloperate::Environment * environment, const std::string & name = "DemoDOFPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoDOFPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::DiscDistributionKernelStage> m_dofShiftStage;  ///< Stage generating DOF shift
    std::unique_ptr<DemoDOFCubeStage>                                m_cubeStage;      ///< Rendering stage
};
