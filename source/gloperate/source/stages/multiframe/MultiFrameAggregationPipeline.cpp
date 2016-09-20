
#include <gloperate/stages/multiframe/MultiFrameAggregationPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/BlitStage.h>
#include <gloperate/stages/multiframe/MultiFrameControlStage.h>
#include <gloperate/stages/multiframe/MultiFrameAggregationStage.h>

namespace gloperate
{

CPPEXPOSE_COMPONENT(MultiFrameAggregationPipeline, gloperate::Stage)


MultiFrameAggregationPipeline::MultiFrameAggregationPipeline(Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_renderFramebufferStage(new BasicFramebufferStage(environment, "BasicFramebufferStage (Renderer)"))
, m_aggregationFramebufferStage(new BasicFramebufferStage(environment, "BasicFramebufferStage (Accumulation)"))
, m_controlStage(new MultiFrameControlStage(environment, "MultiFrameControlStage"))
, m_aggregationStage(new MultiFrameAggregationStage(environment, "MultiFrameAggregationStage"))
, m_blitStage(new BlitStage(environment, "BlitStage"))
, m_renderStage(nullptr)
{
    addStage(m_renderFramebufferStage);
    m_renderFramebufferStage->viewport << renderInterface.deviceViewport;

    addStage(m_aggregationFramebufferStage);
    m_aggregationFramebufferStage->viewport << renderInterface.deviceViewport;

    addStage(m_controlStage);
    m_controlStage->frameNumber << renderInterface.frameCounter;

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
    if (!m_renderStage)
    {
        return;
    }

    Pipeline::onProcess(context);
}

void MultiFrameAggregationPipeline::setFrameRenderer(RenderInterface<Stage> & interface)
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

void MultiFrameAggregationPipeline::setFrameRenderer(RenderInterface<Pipeline> & interface)
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
