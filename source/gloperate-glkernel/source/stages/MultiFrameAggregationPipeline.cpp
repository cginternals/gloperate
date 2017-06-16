
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


namespace
{


template <typename T>
gloperate::Slot<T> * getSlot(gloperate::Stage * stage, const std::string & name)
{
    if (!stage) {
        return nullptr;
    } else {
        return static_cast<gloperate::Slot<T> *>(stage->property(name));
    }
}

template <typename T>
gloperate::Input<T> * getInput(gloperate::Stage * stage, std::function<bool(gloperate::Input<T> *)> callback)
{
    if (!stage)
    {
        return nullptr;
    }

    const auto & inputs = stage->inputs();

    const auto it = std::find_if(inputs.begin(), inputs.end(), [callback](gloperate::AbstractSlot * slot) {
        const auto input = dynamic_cast<gloperate::Input<T> *>(slot);

        if (!input)
        {
            return false;
        }

        return callback(input);
    });

    if (it == inputs.end())
    {
        return nullptr;
    }

    return static_cast<gloperate::Input<T> *>(*it);
}

template <typename T>
void forAllInputs(gloperate::Stage * stage, std::function<void(gloperate::Input<T> *)> callback)
{
    if (!stage)
    {
        return;
    }

    const auto & inputs = stage->inputs();

    for (const auto input : inputs)
    {
        const auto inputT = dynamic_cast<gloperate::Input<T> *>(input);

        if (!inputT)
        {
            continue;
        }

        callback(inputT);
    }
}

template <typename T>
gloperate::Output<T> * getOutput(gloperate::Stage * stage, std::function<bool(gloperate::Output<T> *)> callback)
{
    if (!stage)
    {
        return nullptr;
    }

    const auto & outputs = stage->outputs();

    const auto it = std::find_if(outputs.begin(), outputs.end(), [callback](gloperate::AbstractSlot * slot) {
        const auto output = dynamic_cast<gloperate::Output<T> *>(slot);

        if (!output)
        {
            return false;
        }

        return callback(output);
    });

    if (it == outputs.end())
    {
        return nullptr;
    }

    return static_cast<gloperate::Output<T> *>(*it);
}


}


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
    canvasInterface.addRenderTargetInput(createInput<gloperate::RenderTarget *>("ColorTarget"));
    canvasInterface.addRenderTargetOutput(createOutput<gloperate::RenderTarget *>("ColorTargetOut"));

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
    m_controlStage->viewport << canvasInterface.viewport;

    addStage(m_framePreparationStage.get());

    // m_framePreparationStage->intermediateRenderTarget << ...; // later set by setRenderStage
    m_framePreparationStage->intermediateFrameTexture << m_colorRenderTargetStage->texture;

    addStage(m_aggregationStage.get());
    (*m_aggregationStage->createInput<gloperate::RenderTarget *>("ColorTarget")) << (*canvasInterface.renderTargetInput(0));
    (*canvasInterface.renderTargetOutput(0)) << (*m_aggregationStage->createOutput<gloperate::RenderTarget *>("ColorTargetOut"));
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

/*void MultiFrameAggregationPipeline::onProcess()
{
    if (!m_renderStage)
    {
        return;
    }

    Pipeline::onProcess();
}*/

void MultiFrameAggregationPipeline::setRenderStage(gloperate::Stage * stage)
{
    disconnectRenderStage();

    m_renderStage = stage;

    // Promote viewport information
    auto slotViewport = getInput<glm::vec4>(m_renderStage, [](Input<glm::vec4>* input) { return input->name() == "viewport"; });
    (*slotViewport) << canvasInterface.viewport;

    // Update render stage input render targets
    forAllInputs<gloperate::RenderTarget *>(m_renderStage, [this](Input<gloperate::RenderTarget *> * input) {
        gloperate::RenderTarget * renderTarget = **input;

        switch (renderTarget->attachmentType())
        {
        case gloperate::AttachmentType::Color:
            (*input) << m_colorRenderTargetStage->renderTarget;
            break;
        case gloperate::AttachmentType::Depth:
            (*input) << m_depthStencilRenderTargetStage->renderTarget;
            break;
        case gloperate::AttachmentType::DepthStencil:
            (*input) << m_depthStencilRenderTargetStage->renderTarget;
            break;
        default:
            input->setValue(nullptr);
        }
    });

    // Connect Color Output
    auto slotColorRenderTarget = getOutput<gloperate::RenderTarget *>(m_renderStage, [](Output<gloperate::RenderTarget *> * output) {
        gloperate::RenderTarget * target = **output;

        return target->attachmentType() == gloperate::AttachmentType::Color;
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
