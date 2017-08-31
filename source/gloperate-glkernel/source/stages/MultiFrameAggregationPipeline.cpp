
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
// Inputs
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 64)
// Stages
, m_colorRenderTargetStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "ColorStage"))
, m_depthStencilRenderTargetStage(cppassist::make_unique<gloperate::RenderbufferRenderTargetStage>(environment, "DepthStencilStage"))
, m_controlStage(cppassist::make_unique<MultiFrameControlStage>(environment, "MultiFrameControlStage"))
, m_framePreparationStage(cppassist::make_unique<IntermediateFramePreparationStage>(environment, "IntermediateFramePreparationStage"))
, m_aggregationStage(cppassist::make_unique<MultiFrameAggregationStage>(environment, "MultiFrameAggregationStage"))
// Additional Stages
, m_renderStage(nullptr)
{
    createInput <gloperate::ColorRenderTarget *>("ColorTarget");
    createOutput<gloperate::ColorRenderTarget *>("ColorTargetOut");

    addStage(m_colorRenderTargetStage.get());
    m_colorRenderTargetStage->size << canvasInterface.viewport;
    m_colorRenderTargetStage->format.setValue(gl::GL_RGBA);
    m_colorRenderTargetStage->internalFormat.setValue(gl::GL_RGBA32F);
    m_colorRenderTargetStage->type.setValue(gl::GL_FLOAT);

    addStage(m_depthStencilRenderTargetStage.get());
    m_depthStencilRenderTargetStage->size << canvasInterface.viewport;
    m_depthStencilRenderTargetStage->internalFormat.setValue(gl::GL_DEPTH24_STENCIL8);

    addStage(m_controlStage.get());
    m_controlStage->frameNumber << canvasInterface.frameCounter;
    m_controlStage->multiFrameCount << multiFrameCount;
    m_controlStage->createInput("viewport") << canvasInterface.viewport;

    addStage(m_framePreparationStage.get());

    // m_framePreparationStage->intermediateRenderTarget << ...; // later set by setRenderStage
    m_framePreparationStage->intermediateFrameTexture << m_colorRenderTargetStage->texture;

    addStage(m_aggregationStage.get());
    (*m_aggregationStage->createInput<gloperate::ColorRenderTarget *>("ColorTarget")) << (*canvasInterface.colorRenderTargetInput(0));
    (*canvasInterface.colorRenderTargetOutput(0)) << (*m_aggregationStage->createOutput<gloperate::ColorRenderTarget *>("ColorTargetOut"));
    m_aggregationStage->intermediateFrame << m_framePreparationStage->intermediateFrameTextureOut; // set by setRenderStage
    m_aggregationStage->renderInterface.viewport << canvasInterface.viewport;
    m_aggregationStage->aggregationFactor << m_controlStage->aggregationFactor;

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

    // Promote timeDelta, if applicable
    auto slotTimeDelta = m_renderStage->findInput<float>([](Input<float>* input) { return input->name() == "timeDelta"; });
    if (slotTimeDelta)
        (*slotTimeDelta) << canvasInterface.timeDelta;

    // Provide current frame as frame count
    auto slotFrameCount = m_renderStage->findInput<int>([](Input<int>* input) { return input->name() == "frameCounter"; });
    if (slotFrameCount)
        (*slotFrameCount) << m_controlStage->currentFrame;

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
