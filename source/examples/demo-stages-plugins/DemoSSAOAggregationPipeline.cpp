
#include "DemoSSAOAggregationPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "SSAORenderingPipeline.h"

CPPEXPOSE_COMPONENT(DemoSSAOAggregationPipeline, gloperate::Stage)


DemoSSAOAggregationPipeline::DemoSSAOAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_ssaoPipeline(cppassist::make_unique<SSAORenderingPipeline>(environment))
{
    addStage(m_multiFramePipeline.get());

    m_multiFramePipeline->addStage(m_ssaoPipeline.get());
    //m_ssaoPipeline->multiFrameCount << multiFrameCount;

    // Inputs
    m_multiFramePipeline->aggregationTarget << *createInput<gloperate::ColorRenderTarget *>("Color");

    m_multiFramePipeline->canvasInterface.viewport << canvasInterface.viewport;
    m_multiFramePipeline->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_multiFramePipeline->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_multiFramePipeline->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_multiFramePipeline->multiFrameCount << multiFrameCount;

    // Outputs
    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << m_multiFramePipeline->aggregatedTarget;
}

DemoSSAOAggregationPipeline::~DemoSSAOAggregationPipeline()
{
}
