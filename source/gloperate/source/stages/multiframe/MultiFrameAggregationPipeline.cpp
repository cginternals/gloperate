
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/CustomFramebufferStage.h>
#include <gloperate/stages/base/BlitStage.h>
#include <gloperate/stages/multiframe/MultiFrameControlStage.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationStage.h>

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

    // remove stage from pipeline
    removeStage(m_frameRenderStage);
}


} // namespace gloperate
