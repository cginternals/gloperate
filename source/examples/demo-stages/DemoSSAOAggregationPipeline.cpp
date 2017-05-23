
#include "DemoSSAOAggregationPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "SSAORenderingPipeline.h"

CPPEXPOSE_COMPONENT(DemoSSAOAggregationPipeline, gloperate::Stage)


DemoSSAOAggregationPipeline::DemoSSAOAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_ssaoPipeline(cppassist::make_unique<SSAORenderingPipeline>(environment))
{
    addStage(m_multiFramePipeline.get());

    m_multiFramePipeline->setFrameRenderer(m_ssaoPipeline->renderInterface);

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

DemoSSAOAggregationPipeline::~DemoSSAOAggregationPipeline()
{
}
