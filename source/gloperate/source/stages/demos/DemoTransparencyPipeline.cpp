
#include <gloperate/stages/demos/DemoTransparencyPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/TransparencyKernelStage.h>
#include <gloperate/stages/multiframe/NoiseKernelStage.h>
#include <gloperate/stages/demos/DemoTransparencyStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoTransparencyPipeline, gloperate::Stage)


DemoTransparencyPipeline::DemoTransparencyPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_transparencyKernelStage(new TransparencyKernelStage(environment))
, m_noiseKernelStage(new NoiseKernelStage(environment))
, m_transparencyRenderStage(new DemoTransparencyStage(environment))
{
    addStage(m_transparencyKernelStage);

    addStage(m_noiseKernelStage);
    m_noiseKernelStage->inputDimensions.setValue(2);
    m_noiseKernelStage->outputDimensions.setValue(1);
    m_noiseKernelStage->size.setValue(256);

    addStage(m_transparencyRenderStage);
    m_transparencyRenderStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_transparencyRenderStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_transparencyRenderStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_transparencyRenderStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_transparencyRenderStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_transparencyRenderStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_transparencyRenderStage->transparencyKernel << m_transparencyKernelStage->transparencyMaskTexture;
    m_transparencyRenderStage->noiseKernel << m_noiseKernelStage->noiseTexture;

    renderInterface.rendered << m_transparencyRenderStage->renderInterface.rendered;
}

DemoTransparencyPipeline::~DemoTransparencyPipeline()
{
}


} // namespace gloperate
