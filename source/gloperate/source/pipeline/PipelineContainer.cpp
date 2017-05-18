
#include <gloperate/pipeline/PipelineContainer.h>

#include <gloperate/base/Environment.h>


namespace gloperate
{


PipelineContainer::PipelineContainer(Environment * environment)
: Pipeline(environment, "PipelineContainer", "pipeline")
, deviceViewport ("deviceViewport",  this, glm::vec4(0, 0, 0, 0))
, virtualViewport("virtualViewport", this, glm::vec4(0, 0, 0, 0))
, backgroundColor("backgroundColor", this, glm::vec3(1.0, 1.0, 1.0))
, frameCounter   ("frameCounter",    this, 0)
, timeDelta      ("timeDelta",       this, 0.0f)
, targetFBO      ("targetFBO",       this, nullptr)
, rendered       ("rendered",        this, false)
, m_renderStage(nullptr)
{
}

PipelineContainer::~PipelineContainer()
{
}

Stage * PipelineContainer::renderStage() const
{
    return m_renderStage.get();
}

void PipelineContainer::setRenderStage(std::unique_ptr<Stage> && stage)
{
    // Destroy old render stage
    if (m_renderStage) {
        // Disconnect inputs and outputs from former render stage
        disconnect(m_renderStage.get(), "deviceViewport");
        disconnect(m_renderStage.get(), "virtualViewport");
        disconnect(m_renderStage.get(), "backgroundColor");
        disconnect(m_renderStage.get(), "frameCounter");
        disconnect(m_renderStage.get(), "timeDelta");
        disconnect(m_renderStage.get(), "targetFBO");
        rendered.disconnect();

        // Remove render stage
        removeStage(m_renderStage.get());

        m_renderStage = nullptr;
    }

    // Check parameters
    // [TODO] may also be reasonable on the start of the method
    if (!stage) {
        return;
    }

    // Set new render stage
    this->addStage(stage.get());
    m_renderStage = std::move(stage);

    // Connect inputs and outputs of render stage
    connect(m_renderStage.get(), "deviceViewport",  &deviceViewport);
    connect(m_renderStage.get(), "virtualViewport", &virtualViewport);
    connect(m_renderStage.get(), "backgroundColor", &backgroundColor);
    connect(m_renderStage.get(), "frameCounter",    &frameCounter);
    connect(m_renderStage.get(), "timeDelta",       &timeDelta);
    connect(m_renderStage.get(), "targetFBO",       &targetFBO);
    connect(&rendered, m_renderStage.get(), "rendered");
}

void PipelineContainer::connect(Stage * stage, const std::string & name, AbstractSlot * source)
{
    // Check source data
    if (!source) {
        return;
    }

    // Get input slot
    AbstractSlot * input = dynamic_cast<AbstractSlot *>(stage->property(name));
    if (!input) {
        return;
    }

    // Connect input slot
    input->connect(source);
}

void PipelineContainer::connect(AbstractSlot * input, Stage * stage, const std::string & name)
{
    // Check input slot
    if (!input) {
        return;
    }

    // Get data container
    AbstractSlot * source = static_cast<AbstractSlot *>(stage->property(name));
    if (!source) {
        return;
    }

    // Connect input slot
    input->connect(source);
}

void PipelineContainer::disconnect(Stage * stage, const std::string & name)
{
    // Get input slot
    AbstractSlot * input = dynamic_cast<AbstractSlot *>(stage->property(name));
    if (!input) {
        return;
    }

    // Disconnect input slot
    input->disconnect();
}

std::unique_ptr<Stage> PipelineContainer::obtainRenderStage()
{
    return std::move(m_renderStage);
}


} // namespace gloperate
