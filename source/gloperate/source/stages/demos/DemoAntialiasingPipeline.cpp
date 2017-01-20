
#include <gloperate/stages/demos/DemoAntialiasingPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/SubpixelAntialiasingOffsetStage.h>
#include <gloperate/stages/demos/DemoAntialiasableTriangleStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoAntialiasingPipeline, gloperate::Stage)


DemoAntialiasingPipeline::DemoAntialiasingPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_subpixelStage(new SubpixelAntialiasingOffsetStage(environment))
, m_triangleStage(new DemoAntialiasableTriangleStage(environment))
{
    addStage(m_subpixelStage);
    m_subpixelStage->currentMultiFrame << renderInterface.frameCounter;
    m_subpixelStage->viewport << renderInterface.deviceViewport;

    addStage(m_triangleStage);
    m_triangleStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_triangleStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_triangleStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_triangleStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_triangleStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_triangleStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_triangleStage->subpixelOffset << m_subpixelStage->subPixelOffset;

    renderInterface.rendered << m_triangleStage->renderInterface.rendered;
}

DemoAntialiasingPipeline::~DemoAntialiasingPipeline()
{
}


} // namespace gloperate
