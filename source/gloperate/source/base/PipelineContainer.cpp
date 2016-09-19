
#include <gloperate/base/PipelineContainer.h>

#include <gloperate/base/Environment.h>


namespace gloperate
{


PipelineContainer::PipelineContainer(Environment * environment)
: Pipeline(environment, "pipeline")
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
    return m_renderStage;
}

void PipelineContainer::setRenderStage(Stage * stage)
{
    // Destroy old render stage
    if (m_renderStage) {
        // Disconnect inputs and outputs from former render stage
        disconnect(m_renderStage, "deviceViewport");
        disconnect(m_renderStage, "virtualViewport");
        disconnect(m_renderStage, "backgroundColor");
        disconnect(m_renderStage, "frameCounter");
        disconnect(m_renderStage, "timeDelta");
        disconnect(m_renderStage, "targetFBO");
        rendered.disconnect();

        // Destroy render stage
        destroyStage(m_renderStage);

        m_renderStage = nullptr;
    }

    // Check parameters
    // [TODO] may also be reasonable on the start of the method
    if (!stage) {
        return;
    }

    // Set new render stage
    this->addStage(stage);
    m_renderStage = stage;

    // Connect inputs and outputs of render stage
    connect(m_renderStage, "deviceViewport",  &deviceViewport);
    connect(m_renderStage, "virtualViewport", &virtualViewport);
    connect(m_renderStage, "backgroundColor", &backgroundColor);
    connect(m_renderStage, "frameCounter",    &frameCounter);
    connect(m_renderStage, "timeDelta",       &timeDelta);
    connect(m_renderStage, "targetFBO",       &targetFBO);
    connect(&rendered,     m_renderStage,     "rendered");
}

void PipelineContainer::connect(Stage * stage, const std::string & name, AbstractSlot * source)
{
    // Check source data
    if (!source) {
        return;
    }

    // Get input slot
    AbstractInputSlot * input = dynamic_cast<AbstractInputSlot *>(stage->property(name));
    if (!input) {
        return;
    }

    // Connect input slot
    input->connect(source);
}

void PipelineContainer::connect(AbstractInputSlot * input, Stage * stage, const std::string & name)
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
    AbstractInputSlot * input = dynamic_cast<AbstractInputSlot *>(stage->property(name));
    if (!input) {
        return;
    }

    // Disconnect input slot
    input->disconnect();
}


} // namespace gloperate
