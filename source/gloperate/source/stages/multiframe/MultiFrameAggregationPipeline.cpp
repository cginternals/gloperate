
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/CustomFramebufferStage.h>
#include <gloperate/stages/base/BlitStage.h>
#include <gloperate/stages/multiframe/MultiFrameControlStage.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationStage.h>
#include <gloperate/stages/multiframe/MultiFrameDiscDistributionStage.h>
#include <gloperate/stages/multiframe/NoiseKernelStage.h>
#include <gloperate/stages/multiframe/SSAOKernelStage.h>
#include <gloperate/stages/multiframe/SubpixelAntialiasingOffsetStage.h>
#include <gloperate/stages/multiframe/TransparencyKernelStage.h>

#include <glbinding/gl/enum.h>


namespace gloperate
{

CPPEXPOSE_COMPONENT(MultiFrameAggregationPipeline, gloperate::Stage)


MultiFrameAggregationPipeline::MultiFrameAggregationPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_renderFramebufferStage(new BasicFramebufferStage(environment, "BasicFramebufferStage (Renderer)"))
, m_aggregationFramebufferStage(new CustomFramebufferStage(environment, "CustomFramebufferStage (Accumulation)"))
, m_controlStage(new MultiFrameControlStage(environment, "MultiFrameControlStage"))
, m_aggregationStage(new MultiFrameAggregationStage(environment, "MultiFrameAggregationStage"))
, m_blitStage(new BlitStage(environment, "BlitStage"))
, m_diskDistributionStage(new MultiFrameDiscDistributionStage(environment, "MultiFrameDiscDistributionStage"))
, m_noiseStage(new NoiseKernelStage(environment, "NoiseKernelStage"))
, m_ssaoStage(new SSAOKernelStage(environment, "SSAOKernelStage"))
, m_subpixelStage(new SubpixelAntialiasingOffsetStage(environment, "SubpixelAntialiasingOffsetStage"))
, m_transparencyStage(new TransparencyKernelStage(environment, "TransparencyKernelStage"))
, m_renderStage(nullptr)
{
    addStage(m_renderFramebufferStage);
    m_renderFramebufferStage->viewport << renderInterface.deviceViewport;

    addStage(m_aggregationFramebufferStage);
    m_aggregationFramebufferStage->viewport << renderInterface.deviceViewport;
    m_aggregationFramebufferStage->format.setValue(gl::GL_RGBA);
    m_aggregationFramebufferStage->internalFormat.setValue(gl::GL_RGBA32F);
    m_aggregationFramebufferStage->dataType.setValue(gl::GL_FLOAT);

    addStage(m_controlStage);
    m_controlStage->frameNumber << renderInterface.frameCounter;
    m_controlStage->viewport << renderInterface.deviceViewport;

    addStage(m_aggregationStage);
    m_aggregationStage->aggregationFBO << m_aggregationFramebufferStage->fbo;
    m_aggregationStage->texture << m_renderFramebufferStage->colorTexture;
    m_aggregationStage->viewport << renderInterface.deviceViewport;
    m_aggregationStage->aggregationFactor << m_controlStage->aggregationFactor;

    addStage(m_blitStage);
    m_blitStage->sourceFBO << m_aggregationStage->aggregatedFBO;
    m_blitStage->destinationFBO << renderInterface.targetFBO;
    m_blitStage->sourceViewport << renderInterface.deviceViewport;
    m_blitStage->destinationViewport << renderInterface.deviceViewport;

    renderInterface.rendered << m_blitStage->blitted;

    // to be connected manually
    addStage(m_diskDistributionStage);
    addStage(m_noiseStage);
    addStage(m_ssaoStage);
    addStage(m_subpixelStage);
    addStage(m_transparencyStage);
}

MultiFrameAggregationPipeline::~MultiFrameAggregationPipeline()
{
}

void MultiFrameAggregationPipeline::onProcess(AbstractGLContext * context)
{
    if (!m_renderStage)
    {
        return;
    }

    Pipeline::onProcess(context);
}

void MultiFrameAggregationPipeline::setFrameRenderer(RenderInterface & interface)
{
    if (m_renderStage) removeStage(m_renderStage);

    m_renderStage = interface.rendered.parentStage();
    addStage(m_renderStage);
    interface.deviceViewport << renderInterface.deviceViewport;
    interface.virtualViewport << renderInterface.virtualViewport;
    interface.backgroundColor << renderInterface.backgroundColor;
    interface.frameCounter << renderInterface.frameCounter;
    interface.timeDelta << renderInterface.timeDelta;
    interface.targetFBO << m_renderFramebufferStage->fbo;

    m_aggregationStage->textureRerendered << interface.rendered;
}


} // namespace gloperate
