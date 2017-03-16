
#include <demo-stages/DemoTransparencyPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/multiframe/TransparencyKernelStage.h>
#include <gloperate/stages/multiframe/NoiseKernelStage.h>

#include <demo-stages/DemoTransparencyStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoTransparencyPipeline, gloperate::Stage)


DemoTransparencyPipeline::DemoTransparencyPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_transparencyKernelStage(cppassist::make_unique<TransparencyKernelStage>(environment))
, m_noiseKernelStage(cppassist::make_unique<NoiseKernelStage>(environment))
, m_transparencyRenderStage(cppassist::make_unique<DemoTransparencyStage>(environment))
{
    addStage(m_transparencyKernelStage.get());

    addStage(m_noiseKernelStage.get());
    m_noiseKernelStage->inputDimensions.setValue(2);
    m_noiseKernelStage->outputDimensions.setValue(1);
    m_noiseKernelStage->size.setValue(256);

    addStage(m_transparencyRenderStage.get());
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
