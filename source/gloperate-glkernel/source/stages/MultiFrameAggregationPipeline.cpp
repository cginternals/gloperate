
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/TextureStage.h>
#include <gloperate/stages/base/FramebufferStage.h>
#include <gloperate/stages/base/BlitStage.h>
#include <gloperate-glkernel/stages/MultiFrameControlStage.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationStage.h>

#include <glbinding/gl/enum.h>


namespace gloperate_glkernel
{

CPPEXPOSE_COMPONENT(MultiFrameAggregationPipeline, gloperate::Stage)


MultiFrameAggregationPipeline::MultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, m_renderFramebufferStage(cppassist::make_unique<gloperate::BasicFramebufferStage>(environment, "RenderFramebufferStage"))
, m_aggregationColorTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "ColorTextureStage"))
, m_aggregationDepthTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "DepthTextureStage"))
, m_aggregationFramebufferStage(cppassist::make_unique<gloperate::FramebufferStage>(environment, "AccumulationFramebufferStage"))
, m_controlStage(cppassist::make_unique<MultiFrameControlStage>(environment, "MultiFrameControlStage"))
, m_aggregationStage(cppassist::make_unique<MultiFrameAggregationStage>(environment, "MultiFrameAggregationStage"))
, m_blitStage(cppassist::make_unique<gloperate::BlitStage>(environment, "BlitStage"))
, m_frameRenderStage(nullptr)
{
    addStage(m_renderFramebufferStage.get());
    m_renderFramebufferStage->viewport << renderInterface.deviceViewport;

    addStage(m_aggregationColorTextureStage.get());
    m_aggregationColorTextureStage->size << renderInterface.deviceViewport;
    m_aggregationColorTextureStage->format.setValue(gl::GL_RGBA);
    m_aggregationColorTextureStage->internalFormat.setValue(gl::GL_RGBA32F);
    m_aggregationColorTextureStage->type.setValue(gl::GL_FLOAT);

    addStage(m_aggregationDepthTextureStage.get());
    m_aggregationDepthTextureStage->size << renderInterface.deviceViewport;
    m_aggregationDepthTextureStage->format.setValue(gl::GL_DEPTH_COMPONENT);
    m_aggregationDepthTextureStage->internalFormat.setValue(gl::GL_DEPTH_COMPONENT);
    m_aggregationDepthTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);

    addStage(m_aggregationFramebufferStage.get());
    m_aggregationFramebufferStage->colorTexture << m_aggregationColorTextureStage->renderTarget;
    m_aggregationFramebufferStage->depthTexture << m_aggregationDepthTextureStage->renderTarget;

    addStage(m_controlStage.get());
    m_controlStage->frameNumber << renderInterface.frameCounter;
    m_controlStage->multiFrameCount << multiFrameCount;
    m_controlStage->viewport << renderInterface.deviceViewport;

    addStage(m_aggregationStage.get());
    m_aggregationStage->aggregationFBO << m_aggregationFramebufferStage->fbo;
    m_aggregationStage->texture << m_renderFramebufferStage->colorTexture;
    m_aggregationStage->viewport << renderInterface.deviceViewport;
    m_aggregationStage->aggregationFactor << m_controlStage->aggregationFactor;

    addStage(m_blitStage.get());
    m_blitStage->sourceFBO << m_aggregationStage->aggregatedFBO;
    m_blitStage->destinationFBO << renderInterface.targetFBO;
    m_blitStage->sourceViewport << renderInterface.deviceViewport;
    m_blitStage->destinationViewport << renderInterface.deviceViewport;

    renderInterface.rendered << m_blitStage->blitted;
}

MultiFrameAggregationPipeline::~MultiFrameAggregationPipeline()
{
}

void MultiFrameAggregationPipeline::onProcess(gloperate::AbstractGLContext * context)
{
    if (!m_frameRenderStage)
    {
        return;
    }

    Pipeline::onProcess(context);
}

void MultiFrameAggregationPipeline::setFrameRenderer(gloperate::RenderInterface & interface)
{
    disconnectRenderStage();

    m_frameRenderStage = interface.rendered.parentStage();
    addStage(m_frameRenderStage);

    connectBasicRenderInterface(interface);
}

void MultiFrameAggregationPipeline::connectBasicRenderInterface(gloperate::RenderInterface & interface)
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
