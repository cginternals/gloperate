
#include <gloperate/pipeline/ViewerContainer.h>


namespace gloperate
{


ViewerContainer::ViewerContainer(ViewerContext * viewerContext)
: Pipeline(viewerContext, "Root", nullptr)
, inputs(viewerContext, this)
, outputs(viewerContext, this)
, m_renderStage(nullptr)
{
}

ViewerContainer::~ViewerContainer()
{
}

Stage * ViewerContainer::renderStage() const
{
    return m_renderStage;
}

void ViewerContainer::setRenderStage(Stage * stage)
{
    // Destroy old render stage
    if (m_renderStage) {
        // Disconnect inputs and outputs from former render stage
        disconnect(m_renderStage, "deviceViewport");
        disconnect(m_renderStage, "virtualViewport");
        disconnect(m_renderStage, "backgroundColor");
        disconnect(m_renderStage, "frameCounter");
        disconnect(m_renderStage, "timeDelta");
        outputs.rendered.disconnect();

        // Destroy render stage
        destroyProperty(m_renderStage);
        m_renderStage = nullptr;
    }

    // Check parameters
    if (!stage) {
        return;
    }

    // Set new render stage
    m_renderStage = stage;
    m_renderStage->transferStage(this);

    // Connect inputs and outputs of render stage
    connect(m_renderStage, "deviceViewport",  &inputs.deviceViewport);
    connect(m_renderStage, "virtualViewport", &inputs.virtualViewport);
    connect(m_renderStage, "backgroundColor", &inputs.backgroundColor);
    connect(m_renderStage, "frameCounter",    &inputs.frameCounter);
    connect(m_renderStage, "timeDelta",       &inputs.timeDelta);
    connect(&outputs.rendered, m_renderStage, "rendered");
}

void ViewerContainer::connect(Stage * stage, const std::string & name, const cppexpose::AbstractProperty * source)
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

void ViewerContainer::connect(AbstractInputSlot * input, Stage * stage, const std::string & name)
{
    // Check input slot
    if (!input) {
        return;
    }

    // Get data container
    const cppexpose::AbstractProperty * source = stage->property(name);
    if (!source) {
        return;
    }

    // Connect input slot
    input->connect(source);
}

void ViewerContainer::disconnect(Stage * stage, const std::string & name)
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
