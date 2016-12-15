
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
, multiFrameCount("multiFrameCount", this, 64)
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
, m_frameRenderStage(nullptr)
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
    m_controlStage->multiFrameCount << multiFrameCount;
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

    addStage(m_diskDistributionStage);
    m_diskDistributionStage->currentMultiFrame << m_controlStage->currentFrame;

    addStage(m_noiseStage);

    addStage(m_ssaoStage);
    m_ssaoStage->currentFrame << m_controlStage->currentFrame;

    addStage(m_subpixelStage);
    m_subpixelStage->viewport << renderInterface.deviceViewport;
    m_subpixelStage->currentMultiFrame << m_controlStage->currentFrame;

    addStage(m_transparencyStage);

    renderInterface.rendered << m_blitStage->blitted;
}

MultiFrameAggregationPipeline::~MultiFrameAggregationPipeline()
{
}

void MultiFrameAggregationPipeline::onProcess(AbstractGLContext * context)
{
    if (!m_frameRenderStage)
    {
        return;
    }

    Pipeline::onProcess(context);
}

void MultiFrameAggregationPipeline::setFrameRenderer(RenderInterface & interface)
{
    disconnectRenderStage();

    m_frameRenderStage = interface.rendered.parentStage();
    addStage(m_frameRenderStage);

    connectBasicRenderInterface(interface);
}

void MultiFrameAggregationPipeline::setFrameRenderer(MultiFrameRenderInterface & interface)
{
    setFrameRenderer(static_cast<RenderInterface &>(interface));

    connectMultiFrameRenderInterface(interface);
}

void MultiFrameAggregationPipeline::connectBasicRenderInterface(RenderInterface & interface)
{
    interface.deviceViewport << renderInterface.deviceViewport;
    interface.virtualViewport << renderInterface.virtualViewport;
    interface.backgroundColor << renderInterface.backgroundColor;
    interface.frameCounter << renderInterface.frameCounter;
    interface.timeDelta << renderInterface.timeDelta;
    interface.targetFBO << m_renderFramebufferStage->fbo;

    m_aggregationStage->textureRerendered << interface.rendered;
}

void MultiFrameAggregationPipeline::connectMultiFrameRenderInterface(MultiFrameRenderInterface & interface)
{
    interface.noiseKernel << m_noiseStage->noiseTexture;
    interface.ssaoKernel << m_ssaoStage->kernelTexture;
    interface.ssaoNoise << m_ssaoStage->noiseTexture;
    interface.transparencyKernel << m_transparencyStage->transparencyMaskTexture;
    interface.dofShift << m_diskDistributionStage->value;
    interface.subpixelOffset << m_subpixelStage->subPixelOffset;

    m_ssaoStage->enable << interface.reprocessSSAO;
    m_transparencyStage->reprocess << interface.reprocessTransparency;
    m_diskDistributionStage->isActive << interface.reprocessDOFShift;
    m_noiseStage->inputDimensions << interface.noiseInputDimensions;
    m_noiseStage->outputDimensions << interface.noiseOutputDimensions;
    m_noiseStage->size << interface.noiseKernelSize;
    m_diskDistributionStage->radius << interface.dofRadius;
}

void MultiFrameAggregationPipeline::disconnectRenderStage()
{
    if (!m_frameRenderStage)
    {
        return;
    }

    // disconnect inputs
    for (auto input : m_frameRenderStage->inputs())
    {
        input->disconnect();
    }

    // disconnect outputs by disconnecting their receivers
    m_aggregationStage->textureRerendered.disconnect();

    m_ssaoStage->enable.disconnect();
    m_transparencyStage->reprocess.disconnect();
    m_diskDistributionStage->isActive.disconnect();
    m_noiseStage->inputDimensions.disconnect();
    m_noiseStage->outputDimensions.disconnect();
    m_noiseStage->size.disconnect();
    m_diskDistributionStage->radius.disconnect();

    // remove stage from pipeline
    removeStage(m_frameRenderStage);
}


} // namespace gloperate
