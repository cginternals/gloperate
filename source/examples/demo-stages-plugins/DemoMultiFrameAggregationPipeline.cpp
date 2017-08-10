
#include "DemoMultiFrameAggregationPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "MultiFrameRenderingPipeline.h"

CPPEXPOSE_COMPONENT(DemoMultiFrameAggregationPipeline, gloperate::Stage)


DemoMultiFrameAggregationPipeline::DemoMultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_renderingPipeline(cppassist::make_unique<MultiFrameRenderingPipeline>(environment))
{
    addStage(m_multiFramePipeline.get());

    m_multiFramePipeline->addStage(m_renderingPipeline.get());
    m_renderingPipeline->multiFrameCount << multiFrameCount;

    // Inputs
    *m_multiFramePipeline->canvasInterface.colorRenderTargetInputs()[0] << *createInput<gloperate::ColorRenderTarget *>("Color");

    m_multiFramePipeline->canvasInterface.viewport << canvasInterface.viewport;
    m_multiFramePipeline->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_multiFramePipeline->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_multiFramePipeline->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_multiFramePipeline->multiFrameCount << multiFrameCount;

    // Outputs
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_multiFramePipeline->canvasInterface.colorRenderTargetOutputs()[0];
}

DemoMultiFrameAggregationPipeline::~DemoMultiFrameAggregationPipeline()
{
}
