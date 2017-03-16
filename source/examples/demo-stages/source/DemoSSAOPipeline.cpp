
#include <demo-stages/DemoSSAOPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>

#include <demo-stages/DemoSSAORenderingStage.h>
#include <demo-stages/DemoSSAOPostprocessingStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoSSAOPipeline, gloperate::Stage)


DemoSSAOPipeline::DemoSSAOPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_colorFBOStage(cppassist::make_unique<BasicFramebufferStage>(environment, "Color FBO"))
, m_normalFBOStage(cppassist::make_unique<BasicFramebufferStage>(environment, "Normal FBO"))
, m_kernelStage(cppassist::make_unique<gloperate_glkernel::HemisphereDistributionKernelStage>(environment))
, m_noiseStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_renderingStage(cppassist::make_unique<DemoSSAORenderingStage>(environment))
, m_postprocessingStage(cppassist::make_unique<DemoSSAOPostprocessingStage>(environment))
{
    addStage(m_colorFBOStage.get());
    m_colorFBOStage->viewport << renderInterface.deviceViewport;

    addStage(m_normalFBOStage.get());
    m_normalFBOStage->viewport << renderInterface.deviceViewport;

    addStage(m_kernelStage.get());
    m_kernelStage->kernelSize.setValue(16);

    addStage(m_noiseStage.get());
    m_noiseStage->dimensions.setValue(glm::ivec3(128, 128, 1));

    addStage(m_renderingStage.get());
    m_renderingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_renderingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_renderingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_renderingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_renderingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_renderingStage->renderInterface.targetFBO << m_colorFBOStage->fbo;
    m_renderingStage->normalFBO << m_normalFBOStage->fbo;

    addStage(m_postprocessingStage.get());
    m_postprocessingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_postprocessingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_postprocessingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_postprocessingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_postprocessingStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_postprocessingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_postprocessingStage->colorTexture << m_colorFBOStage->colorTexture;
    m_postprocessingStage->normalTexture << m_normalFBOStage->colorTexture;
    m_postprocessingStage->depthTexture << m_colorFBOStage->depthTexture;
    m_postprocessingStage->ssaoKernel << m_kernelStage->texture;
    m_postprocessingStage->ssaoNoise << m_noiseStage->texture;
    m_postprocessingStage->projectionMatrix << m_renderingStage->projectionMatrix;
    m_postprocessingStage->normalMatrix << m_renderingStage->normalMatrix;
    m_postprocessingStage->sceneRendered << m_renderingStage->renderInterface.rendered;

    renderInterface.rendered << m_postprocessingStage->renderInterface.rendered;
}

DemoSSAOPipeline::~DemoSSAOPipeline()
{
}


} // namespace gloperate
