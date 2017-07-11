
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate
{
    class TextureRenderTargetStage;
    class TextureFromRenderTargetExtractionStage;
}


namespace gloperate_glkernel
{
    class HemisphereDistributionKernelStage;
    class NoiseKernelStage;
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
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer


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
    std::unique_ptr<gloperate::TextureRenderTargetStage>                   m_colorTextureStage;   ///< Stage creating color texture for main rendering
    std::unique_ptr<gloperate::TextureRenderTargetStage>                   m_depthTextureStage;   ///< Stage creating depth texture for main rendering
    std::unique_ptr<gloperate::TextureRenderTargetStage>                   m_normalTextureStage;  ///< Stage creating normal texture for main rendering
    std::unique_ptr<gloperate_glkernel::HemisphereDistributionKernelStage> m_kernelStage;         ///< Stage generating SSAO kernel
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>                  m_noiseStage;          ///< Stage generating SSAO noise
    std::unique_ptr<SSAOSceneRenderingStage>                               m_renderingStage;      ///< Rendering stage
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage>     m_colorTextureExtractionStage;
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage>     m_depthTextureExtractionStage;
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage>     m_normalTextureExtractionStage;
    std::unique_ptr<SSAOApplicationStage>                                  m_postprocessingStage; ///< Postprocessing stage (SSAO applied here)
};
