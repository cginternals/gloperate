
#include <demo-stages/DemoAntialiasingPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>

#include <demo-stages/DemoAntialiasableTriangleStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoAntialiasingPipeline, gloperate::Stage)


DemoAntialiasingPipeline::DemoAntialiasingPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_subpixelStage(new gloperate_glkernel::DiscDistributionKernelStage(environment))
, m_triangleStage(new DemoAntialiasableTriangleStage(environment))
{
    addStage(m_subpixelStage);
    m_subpixelStage->kernelSize << multiFrameCount;
    m_subpixelStage->radius.setValue(0.001f); // guessing inverse height of viewport

    addStage(m_triangleStage);
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


} // namespace gloperate
