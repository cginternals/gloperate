
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate {
    class TextureRenderTargetStage;
}


namespace gloperate_glkernel {
    class TransparencyKernelStage;
    class NoiseKernelStage;
}


class TransparentCirclesStage;


/**
*  @brief
*    Demo pipeline that renders three transparent, overlapping circles onto the screen
*/
class TransparencyRenderingPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TransparencyRenderingPipeline, gloperate::Stage
      , ""                              // Tags
      , ""                              // Icon
      , ""                              // Annotations
      , "Demo pipeline that renders three transparent, overlapping circles onto the screen"
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
    TransparencyRenderingPipeline(gloperate::Environment * environment, const std::string & name = "TransparencyRenderingPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TransparencyRenderingPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::TransparencyKernelStage> m_transparencyKernelStage;  ///< Stage generating transparency kernel
    std::unique_ptr<gloperate_glkernel::NoiseKernelStage>        m_noiseKernelStage;         ///< Stage generating random noise
    std::unique_ptr<gloperate::TextureRenderTargetStage>         m_depthBufferStage;         ///< Stage creating depth buffer for rendering
    std::unique_ptr<TransparentCirclesStage>                     m_transparencyRenderStage;  ///< Rendering stage
};
