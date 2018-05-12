
#include "DemoDOFAggregationPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include "DOFRenderingPipeline.h"

CPPEXPOSE_COMPONENT(DemoDOFAggregationPipeline, gloperate::Stage)


DemoDOFAggregationPipeline::DemoDOFAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_multiFramePipeline(cppassist::make_unique<gloperate_glkernel::MultiFrameAggregationPipeline>(environment))
, m_dofPipeline(cppassist::make_unique<DOFRenderingPipeline>(environment))
{
    addStage(m_multiFramePipeline.get());

    m_multiFramePipeline->addStage(m_dofPipeline.get());
    m_dofPipeline->multiFrameCount << multiFrameCount;

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

DemoDOFAggregationPipeline::~DemoDOFAggregationPipeline()
{
}
