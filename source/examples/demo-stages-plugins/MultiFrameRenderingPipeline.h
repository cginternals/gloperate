
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
    class DiscDistributionKernelStage;
    class HemisphereDistributionKernelStage;
    class NoiseKernelStage;
    class TransparencyKernelStage;
}


class MultiFrameSceneRenderingStage;
class MultiFramePostprocessingStage;


/**
*  @brief
*    Demo pipeline that renders a static triangle onto the screen
*/
class MultiFrameRenderingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameRenderingPipeline, gloperate::Stage
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
    MultiFrameRenderingPipeline(gloperate::Environment * environment, const std::string & name = "MultiFrameRenderingPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameRenderingPipeline();


protected:
    // Stages
    // Custom FBO
    std::unique_ptr<gloperate::TextureRenderTargetStage>                   m_colorTextureStage;   ///< Stage creating color texture for main rendering
    std::unique_ptr<gloperate::TextureRenderTargetStage>                   m_depthTextureStage;   ///< Stage creating depth texture for main rendering
    std::unique_ptr<gloperate::TextureRenderTargetStage>                   m_normalTextureStage;  ///< Stage creating normal texture for main rendering

    // Kernels
    std::unique_ptr<gloperate_glkernel::DiscDistributionKernelStage>       m_subpixelStage;           ///< subpixel offsets for antialiasing
    std::unique_ptr<gloperate_glkernel::DiscDistributionKernelStage>       m_dofShiftStage;           ///< offsets for depth of field
    std::unique_ptr<gloperate_glkernel::HemisphereDistributionKernelStage> m_ssaoKernelStage;         ///< kernel for SSAO
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>                  m_noiseStage;              ///< noise for SSAO & Transparency
    std::unique_ptr<gloperate_glkernel::TransparencyKernelStage>           m_transparencyKernelStage; ///< kernel for transparency

    // Rendering
    std::unique_ptr<MultiFrameSceneRenderingStage>                         m_renderingStage;
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage>     m_colorTextureExtractionStage;
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage>     m_depthTextureExtractionStage;
    std::unique_ptr<gloperate::TextureFromRenderTargetExtractionStage>     m_normalTextureExtractionStage;
    std::unique_ptr<MultiFramePostprocessingStage>                         m_postprocessingStage;
};
