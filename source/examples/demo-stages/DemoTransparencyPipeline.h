
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{

class TransparencyKernelStage;
class NoiseKernelStage;

}


class DemoTransparencyStage;


/**
*  @brief
*    Demo pipeline that renders three transparent, overlapping circles onto the screen
*/
class DemoTransparencyPipeline : public gloperate::Pipeline
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
    DemoTransparencyPipeline(gloperate::Environment * environment, const std::string & name = "DemoTransparencyPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoTransparencyPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate::TransparencyKernelStage> m_transparencyKernelStage;  ///< Stage generating transparency kernel
    std::unique_ptr<gloperate::NoiseKernelStage>        m_noiseKernelStage;         ///< Stage generating random noise
    std::unique_ptr<DemoTransparencyStage>              m_transparencyRenderStage;  ///< Rendering stage
};
