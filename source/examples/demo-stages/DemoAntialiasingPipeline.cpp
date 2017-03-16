
#include "DemoAntialiasingPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/SubpixelAntialiasingOffsetStage.h>

#include "DemoAntialiasableTriangleStage.h"


CPPEXPOSE_COMPONENT(DemoAntialiasingPipeline, gloperate::Stage)


DemoAntialiasingPipeline::DemoAntialiasingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_subpixelStage(cppassist::make_unique<gloperate::SubpixelAntialiasingOffsetStage>(environment))
, m_triangleStage(cppassist::make_unique<DemoAntialiasableTriangleStage>(environment))
{
    addStage(m_subpixelStage.get());
    m_subpixelStage->currentMultiFrame << renderInterface.frameCounter;
    m_subpixelStage->viewport << renderInterface.deviceViewport;

    addStage(m_triangleStage.get());
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
