
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate_glkernel {

class HemisphereDistributionKernelStage;
class NoiseKernelStage;

}

namespace gloperate
{

class BasicFramebufferStage;

}


class DemoSSAORenderingStage;
class DemoSSAOPostprocessingStage;


/**
*  @brief
*    Demo pipeline that renders a simple scene onto the screen
*/
class DemoSSAOPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoSSAOPipeline, gloperate::Stage
      , ""              // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline that renders a simple scene onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer


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
    DemoSSAOPipeline(gloperate::Environment * environment, const std::string & name = "DemoSSAOPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAOPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate::BasicFramebufferStage> m_colorFBOStage;       ///< Color buffer
    std::unique_ptr<gloperate::BasicFramebufferStage> m_normalFBOStage;      ///< Normal buffer
    std::unique_ptr<gloperate_glkernel::HemisphereDistributionKernelStage> m_kernelStage;         ///< Stage generating SSAO kernel
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>                  m_noiseStage;          ///< Stage generating SSAO noise
    std::unique_ptr<DemoSSAORenderingStage>           m_renderingStage;      ///< Rendering stage
    std::unique_ptr<DemoSSAOPostprocessingStage>      m_postprocessingStage; ///< Postprocessing stage (SSAO applied here)
};
