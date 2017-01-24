
#include <gloperate/stages/demos/DemoMultiFramePipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>
#include <gloperate/stages/demos/DemoAntialiasingPipeline.h>
#include <gloperate/stages/demos/DemoDOFPipeline.h>
#include <gloperate/stages/demos/DemoTransparencyPipeline.h>
#include <gloperate/stages/demos/DemoSSAOPipeline.h>

namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoMultiFramePipeline, gloperate::Stage)


DemoMultiFramePipeline::DemoMultiFramePipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_antialiasingPipeline(new DemoAntialiasingPipeline(environment))
, m_dofPipeline(new DemoDOFPipeline(environment))
, m_transparencyPipeline(new DemoTransparencyPipeline(environment))
, m_ssaoPipeline(new DemoSSAOPipeline(environment))
, m_multiFramePipeline(new MultiFrameAggregationPipeline(environment))
{
    addStage(m_multiFramePipeline);

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
