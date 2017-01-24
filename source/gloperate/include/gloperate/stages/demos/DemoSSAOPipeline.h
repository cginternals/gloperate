
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class BasicFramebufferStage;
class SSAOKernelStage;
class DemoSSAORenderingStage;
class DemoSSAOPostprocessingStage;


/**
*  @brief
*    Demo pipeline that renders a simple scene onto the screen
*/
class GLOPERATE_API DemoSSAOPipeline : public Pipeline
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
    RenderInterface renderInterface; ///< Interface for rendering into a viewer


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
    DemoSSAOPipeline(Environment * environment, const std::string & name = "DemoSSAOPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAOPipeline();


protected:
    // Stages
    BasicFramebufferStage       * m_colorFBOStage;       ///< Color buffer
    BasicFramebufferStage       * m_normalFBOStage;      ///< Normal buffer
    SSAOKernelStage             * m_kernelStage;         ///< Stage generating SSAO kernel
    DemoSSAORenderingStage      * m_renderingStage;      ///< Rendering stage
    DemoSSAOPostprocessingStage * m_postprocessingStage; ///< Postprocessing stage (SSAO applied here)
};


} // namespace gloperate
