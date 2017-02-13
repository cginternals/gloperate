
#include <gloperate/stages/demos/DemoSSAOPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/multiframe/SSAOKernelStage.h>
#include <gloperate/stages/demos/DemoSSAORenderingStage.h>
#include <gloperate/stages/demos/DemoSSAOPostprocessingStage.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(DemoSSAOPipeline, gloperate::Stage)


DemoSSAOPipeline::DemoSSAOPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_colorFBOStage(new BasicFramebufferStage(environment, "Color FBO"))
, m_normalFBOStage(new BasicFramebufferStage(environment, "Normal FBO"))
, m_kernelStage(new SSAOKernelStage(environment))
, m_renderingStage(new DemoSSAORenderingStage(environment))
, m_postprocessingStage(new DemoSSAOPostprocessingStage(environment))
{
    addStage(m_colorFBOStage);
    m_colorFBOStage->viewport << renderInterface.deviceViewport;

    addStage(m_normalFBOStage);
    m_normalFBOStage->viewport << renderInterface.deviceViewport;

    addStage(m_kernelStage);
    m_kernelStage->currentFrame << renderInterface.frameCounter;
    m_kernelStage->noiseSize.setValue(128);
    m_kernelStage->kernelSize.setValue(16);

    addStage(m_renderingStage);
    m_renderingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_renderingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_renderingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_renderingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_renderingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_renderingStage->renderInterface.targetFBO << m_colorFBOStage->fbo;
    m_renderingStage->normalFBO << m_normalFBOStage->fbo;

    addStage(m_postprocessingStage);
    m_postprocessingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_postprocessingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_postprocessingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_postprocessingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_postprocessingStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_postprocessingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_postprocessingStage->colorTexture << m_colorFBOStage->colorTexture;
    m_postprocessingStage->normalTexture << m_normalFBOStage->colorTexture;
    m_postprocessingStage->depthTexture << m_colorFBOStage->depthTexture;
    m_postprocessingStage->ssaoKernel << m_kernelStage->kernelTexture;
    m_postprocessingStage->ssaoNoise << m_kernelStage->noiseTexture;
    m_postprocessingStage->projectionMatrix << m_renderingStage->projectionMatrix;
    m_postprocessingStage->normalMatrix << m_renderingStage->normalMatrix;
    m_postprocessingStage->sceneRendered << m_renderingStage->renderInterface.rendered;

    renderInterface.rendered << m_postprocessingStage->renderInterface.rendered;
}

DemoSSAOPipeline::~DemoSSAOPipeline()
{
}


} // namespace gloperate
