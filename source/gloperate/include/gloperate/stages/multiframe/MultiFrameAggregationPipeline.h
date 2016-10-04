
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


class BasicFramebufferStage;
class CustomFramebufferStage;
class BlitStage;
class MultiFrameControlStage;
class MultiFrameAggregationStage;


class GLOPERATE_API MultiFrameAggregationPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameAggregationPipeline, gloperate::Stage
      , ""
      , ""
      , ""
      , "Pipeline that aggregates multiple frames rendered by the given Stage/Pipeline"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    RenderInterface renderInterface; ///< Interface for rendering into a viewer


public:
    MultiFrameAggregationPipeline(Environment * environment, const std::string & name = "MultiFrameAggregationPipeline");

    virtual ~MultiFrameAggregationPipeline();

    void setFrameRenderer(RenderInterface & interface);


protected:
    virtual void onProcess(AbstractGLContext * context) override;


protected:
    // Stages
    BasicFramebufferStage      * m_renderFramebufferStage;
    CustomFramebufferStage     * m_aggregationFramebufferStage;
    MultiFrameControlStage     * m_controlStage;
    MultiFrameAggregationStage * m_aggregationStage;
    BlitStage                  * m_blitStage;

    Stage                      * m_renderStage;
};


} // namespace gloperate
