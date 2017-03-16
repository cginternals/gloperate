
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <demo-stages/demo-stages_api.h>

namespace gloperate_glkernel {

class TransparencyKernelStage;
class NoiseKernelStage;

}

namespace gloperate
{


class DemoTransparencyStage;


/**
*  @brief
*    Demo pipeline that renders three transparent, overlapping circles onto the screen
*/
class DEMO_STAGES_API DemoTransparencyPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoTransparencyPipeline, gloperate::Stage
      , ""              // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline that renders three transparent, overlapping circles onto the screen"
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
    DemoTransparencyPipeline(Environment * environment, const std::string & name = "DemoTransparencyPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoTransparencyPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::TransparencyKernelStage> m_transparencyKernelStage;  ///< Stage generating transparency kernel
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>        m_noiseKernelStage;         ///< Stage generating random noise
    std::unique_ptr<DemoTransparencyStage>   m_transparencyRenderStage;  ///< Rendering stage
};


} // namespace gloperate
