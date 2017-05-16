
#include "DemoTransparencyAggregationPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "DemoTransparencyPipeline.h"

CPPEXPOSE_COMPONENT(DemoTransparencyAggregationPipeline, gloperate::Stage)


DemoTransparencyAggregationPipeline::DemoTransparencyAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_transparencyPipeline(cppassist::make_unique<DemoTransparencyPipeline>(environment))
{
    addStage(m_multiFramePipeline.get());

    m_multiFramePipeline->setFrameRenderer(m_transparencyPipeline->renderInterface);

    // Inputs
    m_multiFramePipeline->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_multiFramePipeline->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_multiFramePipeline->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_multiFramePipeline->renderInterface.frameCounter << renderInterface.frameCounter;
    m_multiFramePipeline->renderInterface.timeDelta << renderInterface.timeDelta;
    m_multiFramePipeline->renderInterface.targetFBO << renderInterface.targetFBO;
    m_multiFramePipeline->multiFrameCount << multiFrameCount;

    // Outputs
    renderInterface.rendered << m_multiFramePipeline->renderInterface.rendered;
}

DemoTransparencyAggregationPipeline::~DemoTransparencyAggregationPipeline()
{
}
