
#include "DemoMultiFrameAggregationPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "MultiFrameRenderingPipeline.h"

CPPEXPOSE_COMPONENT(DemoMultiFrameAggregationPipeline, gloperate::Stage)


DemoMultiFrameAggregationPipeline::DemoMultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 256)
, m_aggregationPipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_renderingPipeline(cppassist::make_unique<MultiFrameRenderingPipeline>(environment))
{
    addStage(m_aggregationPipeline.get());

    m_aggregationPipeline->setFrameRenderer(m_renderingPipeline->renderInterface);
    m_renderingPipeline->multiFrameCount << multiFrameCount;

    // Inputs
    m_aggregationPipeline->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_aggregationPipeline->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_aggregationPipeline->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_aggregationPipeline->renderInterface.frameCounter << renderInterface.frameCounter;
    m_aggregationPipeline->renderInterface.timeDelta << renderInterface.timeDelta;
    m_aggregationPipeline->renderInterface.targetFBO << renderInterface.targetFBO;
    m_aggregationPipeline->multiFrameCount << multiFrameCount;

    // Outputs
    renderInterface.rendered << m_aggregationPipeline->renderInterface.rendered;
}

DemoMultiFrameAggregationPipeline::~DemoMultiFrameAggregationPipeline()
{
}
