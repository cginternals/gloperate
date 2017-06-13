
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate_glkernel
{
    class HemisphereDistributionKernelStage;
    class NoiseKernelStage;
}

namespace gloperate
{
    class FramebufferStage;
    class TextureStage;
}

class SSAOSceneRenderingStage;
class SSAOApplicationStage;


/**
*  @brief
*    Demo pipeline that renders a simple scene onto the screen
*/
class SSAORenderingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        SSAORenderingPipeline, gloperate::Stage
      , ""                      // Tags
      , ""                      // Icon
      , ""                      // Annotations
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
    SSAORenderingPipeline(gloperate::Environment * environment, const std::string & name = "SSAORenderingPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~SSAORenderingPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate::TextureStage>                               m_colorTextureStage;   ///< Stage creating color texture for main rendering
    std::unique_ptr<gloperate::TextureStage>                               m_depthTextureStage;   ///< Stage creating depth texture for main rendering
    std::unique_ptr<gloperate::TextureStage>                               m_normalTextureStage;  ///< Stage creating normal texture for main rendering
    std::unique_ptr<gloperate::FramebufferStage>                           m_fboStage;            ///< Stage creating FBO for main rendering
    std::unique_ptr<gloperate_glkernel::HemisphereDistributionKernelStage> m_kernelStage;         ///< Stage generating SSAO kernel
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>                  m_noiseStage;          ///< Stage generating SSAO noise
    std::unique_ptr<SSAOSceneRenderingStage>                               m_renderingStage;      ///< Rendering stage
    std::unique_ptr<SSAOApplicationStage>                                  m_postprocessingStage; ///< Postprocessing stage (SSAO applied here)
};
