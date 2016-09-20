
#include <gloperate/stages/demos/DemoMultiFramePipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/demos/DemoStage.h>

namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoMultiFramePipeline, gloperate::Stage)


DemoMultiFramePipeline::DemoMultiFramePipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_frameRenderStage(new DemoStage(environment, "TriangleStage"))
, m_multiFramePipeline(new MultiFrameAggregationPipeline(environment, "MultiFrameAggregationStage"))
{
    addStage(m_multiFramePipeline);
    m_multiFramePipeline->setFrameRenderer(m_frameRenderStage->renderInterface);

    // Inputs
    m_multiFramePipeline->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_multiFramePipeline->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_multiFramePipeline->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_multiFramePipeline->renderInterface.frameCounter << renderInterface.frameCounter;
    m_multiFramePipeline->renderInterface.timeDelta << renderInterface.timeDelta;
    m_multiFramePipeline->renderInterface.targetFBO << renderInterface.targetFBO;

    // Outputs
    renderInterface.rendered << m_multiFramePipeline->renderInterface.rendered;

}

DemoMultiFramePipeline::~DemoMultiFramePipeline()
{
}


} // namespace gloperate
