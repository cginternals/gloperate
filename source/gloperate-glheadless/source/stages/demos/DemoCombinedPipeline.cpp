
#include <gloperate-glheadless/stages/demos/DemoCombinedPipeline.h>

#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/demos/DemoTriangleStage.h>
#include <gloperate/stages/demos/DemoTimerStage.h>

#include <gloperate-glheadless/stages/demos/DemoOffscreenPipeline.h>


using namespace gloperate;


namespace gloperate_glheadless {


CPPEXPOSE_COMPONENT(DemoCombinedPipeline, gloperate::Stage)


DemoCombinedPipeline::DemoCombinedPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_offscreenStage(new DemoOffscreenPipeline(environment, "OffscreenStage"))
, m_timerStage(new DemoTimerStage(environment, "TimerStage"))
, m_triangleStage(new DemoTriangleStage(environment, "Triangle Stage"))
{
    // Offscreen stage
    addStage(m_offscreenStage);
    m_offscreenStage->viewport.setValue({ 0.0f, 0.0f, 400.0f, 400.0f });
    m_offscreenStage->backgroundColor.setValue({ 0.3, 0.4, 1.0 });
    m_offscreenStage->timeDelta << renderInterface.timeDelta;
    m_offscreenStage->rendered.setRequired(true);

    // Timer stage
    addStage(m_timerStage);
    m_timerStage->timeDelta << renderInterface.timeDelta;

    // Triangle stage
    addStage(m_triangleStage);
    m_triangleStage->renderInterface.deviceViewport  << renderInterface.deviceViewport;
    m_triangleStage->renderInterface.targetFBO       << renderInterface.targetFBO;
    m_triangleStage->texture                         << m_offscreenStage->colorTexture;
    m_triangleStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_triangleStage->angle                           << m_timerStage->virtualTime;

    // Connect outputs
    renderInterface.rendered << m_triangleStage->renderInterface.rendered;
}


DemoCombinedPipeline::~DemoCombinedPipeline()
{
}


} // gloperate_glheadless
