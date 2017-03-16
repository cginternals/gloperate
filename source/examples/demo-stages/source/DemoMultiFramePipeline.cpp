
#include <demo-stages/DemoMultiFramePipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>

#include <demo-stages/DemoAntialiasingPipeline.h>
#include <demo-stages/DemoDOFPipeline.h>
#include <demo-stages/DemoTransparencyPipeline.h>
#include <demo-stages/DemoSSAOPipeline.h>

namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoMultiFramePipeline, gloperate::Stage)


DemoMultiFramePipeline::DemoMultiFramePipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_multiFramePipeline(cppassist::make_unique<MultiFrameAggregationPipeline>(environment))
, m_antialiasingPipeline(cppassist::make_unique<DemoAntialiasingPipeline>(environment))
, m_dofPipeline(cppassist::make_unique<DemoDOFPipeline>(environment))
, m_transparencyPipeline(cppassist::make_unique<DemoTransparencyPipeline>(environment))
, m_ssaoPipeline(cppassist::make_unique<DemoSSAOPipeline>(environment))
{
    addStage(m_multiFramePipeline.get());

    //m_multiFramePipeline->setFrameRenderer(m_antialiasingPipeline->renderInterface);
    //m_multiFramePipeline->setFrameRenderer(m_dofPipeline->renderInterface);
    //m_multiFramePipeline->setFrameRenderer(m_transparencyPipeline->renderInterface);
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

DemoMultiFramePipeline::~DemoMultiFramePipeline()
{
}


} // namespace gloperate
