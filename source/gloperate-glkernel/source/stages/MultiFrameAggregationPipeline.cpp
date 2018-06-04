
#include <gloperate-glkernel/stages/MultiFrameAggregationPipeline.h>

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/TextureRenderTargetStage.h>
#include <gloperate/stages/base/RenderbufferRenderTargetStage.h>
#include <gloperate/stages/base/BlitStage.h>

#include <gloperate-glkernel/stages/MultiFrameControlStage.h>
#include <gloperate-glkernel/stages/MultiFrameAggregationStage.h>
#include <gloperate-glkernel/stages/IntermediateFramePreparationStage.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(MultiFrameAggregationPipeline, gloperate::Stage)


MultiFrameAggregationPipeline::MultiFrameAggregationPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
// Inputs & Outputs
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
, aggregationTarget("aggregationTarget", this)
, aggregatedTarget("aggregatedTarget", this)
// Stages
, m_colorRenderTargetStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "ColorStage"))
, m_aggregationRenderTargetStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "AggregationBufferStage"))
, m_depthStencilRenderTargetStage(cppassist::make_unique<gloperate::RenderbufferRenderTargetStage>(environment, "DepthStencilStage"))
, m_controlStage(cppassist::make_unique<MultiFrameControlStage>(environment, "MultiFrameControlStage"))
, m_framePreparationStage(cppassist::make_unique<IntermediateFramePreparationStage>(environment, "IntermediateFramePreparationStage"))
, m_aggregationStage(cppassist::make_unique<MultiFrameAggregationStage>(environment, "MultiFrameAggregationStage"))
, m_blitStage(cppassist::make_unique<gloperate::BlitStage>(environment, "BlitStage"))
// Additional Stages
, m_renderStage(nullptr)
{
    addStage(m_colorRenderTargetStage.get());
    m_colorRenderTargetStage->size << canvasInterface.viewport;
    m_colorRenderTargetStage->format.setValue(gl::GL_RGBA);
    m_colorRenderTargetStage->internalFormat.setValue(gl::GL_RGBA8);
    m_colorRenderTargetStage->type.setValue(gl::GL_INT);

    addStage(m_aggregationRenderTargetStage.get());
    m_aggregationRenderTargetStage->size << canvasInterface.viewport;
    m_aggregationRenderTargetStage->format = gl::GL_RGBA;
    m_aggregationRenderTargetStage->internalFormat = gl::GL_RGBA32F;
    m_aggregationRenderTargetStage->type = gl::GL_FLOAT;

    addStage(m_depthStencilRenderTargetStage.get());
    m_depthStencilRenderTargetStage->size << canvasInterface.viewport;
    m_depthStencilRenderTargetStage->internalFormat.setValue(gl::GL_DEPTH24_STENCIL8);

    addStage(m_controlStage.get());
    m_controlStage->frameNumber << canvasInterface.frameCounter;
    m_controlStage->multiFrameCount << multiFrameCount;
    m_controlStage->viewport << canvasInterface.viewport;

    addStage(m_framePreparationStage.get());

    // m_framePreparationStage->intermediateRenderTarget << ...; // later set by setRenderStage
    m_framePreparationStage->intermediateFrameTexture << m_colorRenderTargetStage->texture;

    addStage(m_aggregationStage.get());
    m_aggregationStage->createInput("ColorTarget") << m_aggregationRenderTargetStage->colorRenderTarget;
    m_aggregationStage->intermediateFrame << m_framePreparationStage->intermediateFrameTextureOut; // set by setRenderStage
    m_aggregationStage->renderInterface.viewport << canvasInterface.viewport;
    m_aggregationStage->aggregationFactor << m_controlStage->aggregationFactor;

    addStage(m_blitStage.get());
    m_blitStage->source << *m_aggregationStage->createOutput<gloperate::ColorRenderTarget *>("ColorTargetOut");
    m_blitStage->sourceViewport << canvasInterface.viewport;
    m_blitStage->target << aggregationTarget;
    m_blitStage->targetViewport << canvasInterface.viewport;

    aggregatedTarget << m_blitStage->targetOut;

    stageAdded.connect([this](Stage * stage) {
        setRenderStage(stage);
    });

    stageRemoved.connect([this](Stage * stage) {
        if (m_renderStage == stage)
        {
            disconnectRenderStage();
        }
    });
}

MultiFrameAggregationPipeline::~MultiFrameAggregationPipeline()
{
}

void MultiFrameAggregationPipeline::setRenderStage(gloperate::Stage * stage)
{
    disconnectRenderStage();

    m_renderStage = stage;

    // Promote viewport information
    auto slotViewport = m_renderStage->findInput<glm::vec4>([](Input<glm::vec4>* input) { return input->name() == "viewport"; });
    (*slotViewport) << canvasInterface.viewport;

    // Update render stage input render targets
    m_renderStage->forAllInputs<gloperate::ColorRenderTarget *>([this](Input<gloperate::ColorRenderTarget *> * input) {
        (*input) << m_colorRenderTargetStage->colorRenderTarget;
    });
    m_renderStage->forAllInputs<gloperate::DepthRenderTarget *>([this](Input<gloperate::DepthRenderTarget *> * input) {
        (*input) << m_depthStencilRenderTargetStage->depthRenderTarget;
    });
    m_renderStage->forAllInputs<gloperate::StencilRenderTarget *>([this](Input<gloperate::StencilRenderTarget *> * input) {
        (*input) << m_depthStencilRenderTargetStage->stencilRenderTarget;
    });


    // Connect Color Output
    auto slotColorRenderTarget = m_renderStage->findOutput<gloperate::ColorRenderTarget *>([](Output<gloperate::ColorRenderTarget *> * /*output*/) {
        return true;
    });

    if (slotColorRenderTarget)
    {
        m_framePreparationStage->intermediateRenderTarget << (*slotColorRenderTarget);
    }


    // Connect other inputs if present
    auto slotFrameCounter = dynamic_cast<Input<int>*>(m_renderStage->input("frameCounter"));
    if (slotFrameCounter)
    {
        *slotFrameCounter << m_controlStage->currentFrame;
    }
}

void MultiFrameAggregationPipeline::disconnectRenderStage()
{
    if (!m_renderStage)
    {
        return;
    }

    m_renderStage = nullptr;
}


} // namespace gloperate_glkernel
