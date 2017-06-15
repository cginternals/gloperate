
#include "DemoTransparencyPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate-glkernel/stages/TransparencyKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>

#include "DemoTransparencyStage.h"


CPPEXPOSE_COMPONENT(DemoTransparencyPipeline, gloperate::Stage)


DemoTransparencyPipeline::DemoTransparencyPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_transparencyKernelStage(cppassist::make_unique<gloperate_glkernel::TransparencyKernelStage>(environment))
, m_noiseKernelStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_transparencyRenderStage(cppassist::make_unique<DemoTransparencyStage>(environment))
{
    addStage(m_transparencyKernelStage.get());
    m_transparencyKernelStage->kernelSize.setValue(glm::ivec2(256, 256));

    addStage(m_noiseKernelStage.get());
    m_noiseKernelStage->dimensions.setValue(glm::ivec3(64, 64, 64));

    addStage(m_transparencyRenderStage.get());
    m_transparencyRenderStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_transparencyRenderStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_transparencyRenderStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_transparencyRenderStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_transparencyRenderStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_transparencyRenderStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_transparencyRenderStage->transparencyKernel << m_transparencyKernelStage->texture;
    m_transparencyRenderStage->noiseKernel << m_noiseKernelStage->texture;

    renderInterface.rendered << m_transparencyRenderStage->renderInterface.rendered;
}

DemoTransparencyPipeline::~DemoTransparencyPipeline()
{
}