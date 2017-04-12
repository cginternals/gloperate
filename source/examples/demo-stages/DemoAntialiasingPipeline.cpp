
#include "DemoAntialiasingPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include "DemoAntialiasableTriangleStage.h"


CPPEXPOSE_COMPONENT(DemoAntialiasingPipeline, gloperate::Stage)


DemoAntialiasingPipeline::DemoAntialiasingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_subpixelStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_triangleStage(cppassist::make_unique<DemoAntialiasableTriangleStage>(environment))
{
    addStage(m_subpixelStage.get());
    m_subpixelStage->kernelSize << multiFrameCount;
    m_subpixelStage->radius.setValue(0.001f); // guessing inverse height of viewport

    addStage(m_triangleStage.get());
    m_triangleStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_triangleStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_triangleStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_triangleStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_triangleStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_triangleStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_triangleStage->subpixelOffsets << m_subpixelStage->kernel;

    renderInterface.rendered << m_triangleStage->renderInterface.rendered;
}

DemoAntialiasingPipeline::~DemoAntialiasingPipeline()
{
}
