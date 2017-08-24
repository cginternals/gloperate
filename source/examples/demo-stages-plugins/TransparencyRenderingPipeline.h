
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/NamedString.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate {
    class Drawable;

    class ProgramStage;
    class ClearStage;
    class RenderPassStage;
    class RasterizationStage;
}


namespace gloperate_glkernel {
    class TransparencyKernelStage;
}


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
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::TransparencyKernelStage> m_transparencyKernelStage;  ///< Stage generating transparency kernel
    std::unique_ptr<gloperate::ProgramStage>                     m_programStage;
    std::unique_ptr<gloperate::RenderPassStage>                  m_renderPassStage;
    std::unique_ptr<gloperate::ClearStage>                       m_clearStage;
    std::unique_ptr<gloperate::RasterizationStage>               m_rasterizationStage;

    // Rendering ressources
    std::unique_ptr<gloperate::Drawable>                         m_drawable;
    std::unique_ptr<globjects::NamedString>                      m_randomIncludeNamedString;
};
