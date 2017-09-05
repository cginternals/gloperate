
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace globjects
{
    class NamedString;
}


namespace gloperate
{
    class TextureRenderTargetStage;
    class TextureFromRenderTargetExtractionStage;
    class Quad;
    class Camera;
    class ProgramStage;
    class RenderPassStage;
    class RasterizationStage;
    class ClearStage;
}


namespace gloperate_glkernel
{
    class DiscDistributionKernelStage;
    class HemisphereDistributionKernelStage;
    class NoiseKernelStage;
    class TransparencyKernelStage;
}


class GeometryImporterStage;


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
    Input<gloperate::Camera *> camera;          ///< Camera provided by navigation

    Input<bool> useAntialiasing;                ///< Flag enabling antialiasing effect
    Input<bool> useDOF;                         ///< Flag enabling depth of field effect
    Input<bool> useSSAO;                        ///< Flag enabling ssao effect
    Input<bool> useTransparency;                ///< Flag enabling transparency effect

    Input<float> transparency_alpha;            ///< Alpha value for transparency


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
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context);
    virtual void onContextDeinit(gloperate::AbstractGLContext * context);


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
    std::unique_ptr<GeometryImporterStage>                                 m_renderGeometryStage;              ///< geometry for rendering step
    std::unique_ptr<gloperate::ProgramStage>                               m_renderProgramStage;               ///< shader program for rendering step
    std::unique_ptr<gloperate::RenderPassStage>                            m_renderPassStage;                  ///< render pass for rendering step
    std::unique_ptr<gloperate::ClearStage>                                 m_renderClearStage;                 ///< FBO clear for rendering step
    std::unique_ptr<gloperate::RasterizationStage>                         m_renderRasterizationStage;         ///< rasterization for rendering step
    std::unique_ptr<gloperate::ProgramStage>                               m_postprocessingProgramStage;       ///< shader program for postprocessing step
    std::unique_ptr<gloperate::RenderPassStage>                            m_postprocessingPassStage;          ///< render pass for postprocessing step
    std::unique_ptr<gloperate::ClearStage>                                 m_postprocessingClearStage;         ///< FBO clear for postprocessing step
    std::unique_ptr<gloperate::RasterizationStage>                         m_postprocessingRasterizationStage; ///< rasterization for postprocessing step

    // Rendering ressources
    std::unique_ptr<globjects::NamedString>                                m_randomNamedString; ///< named string for shader include
    std::unique_ptr<globjects::NamedString>                                m_ssaoNamedString;   ///< named string for shader include
    std::unique_ptr<gloperate::Camera>                                     m_camera;            ///< camera for rendering step
    std::unique_ptr<gloperate::Quad>                                       m_quad;              ///< geometry for postprocessing step
};
