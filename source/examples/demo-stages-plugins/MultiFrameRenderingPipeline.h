
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class NamedString;
}


namespace gloperate
{
    class ScreenAlignedQuad;
    class Camera;

    class FramebufferStage;
    class TextureStage;
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
    gloperate::RenderInterface renderInterface; ///< Interface for rendering into a viewer

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
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context);
    virtual void onContextDeinit(gloperate::AbstractGLContext * context);


protected:
    // Stages
    // Custom FBO
    std::unique_ptr<gloperate::TextureStage>                               m_colorTextureStage;   ///< Stage creating color texture for main rendering
    std::unique_ptr<gloperate::TextureStage>                               m_depthTextureStage;   ///< Stage creating depth texture for main rendering
    std::unique_ptr<gloperate::TextureStage>                               m_normalTextureStage;  ///< Stage creating normal texture for main rendering
    std::unique_ptr<gloperate::FramebufferStage>                           m_fboStage;            ///< Stage creating FBO for main rendering

    // Kernels
    std::unique_ptr<gloperate_glkernel::DiscDistributionKernelStage>       m_subpixelStage;           ///< subpixel offsets for antialiasing
    std::unique_ptr<gloperate_glkernel::DiscDistributionKernelStage>       m_dofShiftStage;           ///< offsets for depth of field
    std::unique_ptr<gloperate_glkernel::HemisphereDistributionKernelStage> m_ssaoKernelStage;         ///< kernel for SSAO
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>                  m_noiseStage;              ///< noise for SSAO & Transparency
    std::unique_ptr<gloperate_glkernel::TransparencyKernelStage>           m_transparencyKernelStage; ///< kernel for transparency

    // Rendering
    std::unique_ptr<GeometryImporterStage>                                 m_renderGeometryStage;              ///< geometry for rendering step
    std::unique_ptr<gloperate::ProgramStage>                               m_renderProgramStage;               ///< shader program for rendering step
    std::unique_ptr<gloperate::Camera>                                     m_camera;                           ///< camera for rendering step
    std::unique_ptr<gloperate::RenderPassStage>                            m_renderPassStage;                  ///< render pass for rendering step
    std::unique_ptr<gloperate::ClearStage>                                 m_renderClearStage;                 ///< FBO clear for rendering step
    std::unique_ptr<gloperate::RasterizationStage>                         m_renderRasterizationStage;         ///< rasterization for rendering step
    std::unique_ptr<gloperate::ScreenAlignedQuad>                          m_screenAlignedQuad;                ///< geometry for postprocessing step
    std::unique_ptr<globjects::NamedString>                                m_ssaoNamedString;                  ///< named string for shader include
    std::unique_ptr<gloperate::ProgramStage>                               m_postprocessingProgramStage;       ///< shader program for postprocessing step
    std::unique_ptr<gloperate::RenderPassStage>                            m_postprocessingPassStage;          ///< render pass for postprocessing step
    std::unique_ptr<gloperate::ClearStage>                                 m_postprocessingClearStage;         ///< FBO clear for postprocessing step
    std::unique_ptr<gloperate::RasterizationStage>                         m_postprocessingRasterizationStage; ///< rasterization for postprocessing step
};
