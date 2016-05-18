
#include <gloperate/pipeline/Stage.h>

#include <algorithm>

#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/AbstractInputSlot.h>
#include <gloperate/pipeline/AbstractData.h>


namespace gloperate
{


Stage::Stage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: cppexpose::Object(name, parent)
, m_viewerContext(viewerContext)
, m_parentPipeline(parent)
{
    if (parent) {
        parent->registerStage(this);
    }
}

Stage::~Stage()
{
    if (m_parentPipeline) {
        m_parentPipeline->unregisterStage(this);
    }
}

bool Stage::isPipeline() const
{
    return false;
}

ViewerContext * Stage::viewerContext() const
{
    return m_viewerContext;
}

Pipeline * Stage::parentPipeline() const
{
    return m_parentPipeline;
}

void Stage::transferStage(Pipeline * parent)
{
    // Abort if the parent pipeline is already set or parameter is empty
    if (m_parentPipeline || !parent) {
        return;
    }

    // Set new parent
    m_parentPipeline = parent;
    m_parentPipeline->registerStage(this);
    initProperty(m_parentPipeline, m_name);
    parent->takeOwnership(m_parentPipeline);
}

void Stage::initContext(AbstractGLContext * context)
{
    onContextInit(context);
}

void Stage::deinitContext(AbstractGLContext * context)
{
    onContextDeinit(context);
}

void Stage::process(AbstractGLContext * context)
{
    onProcess(context);
}

const std::vector<AbstractInputSlot *> & Stage::inputs() const
{
    return m_inputs;
}

const AbstractInputSlot * Stage::input(const std::string & name) const
{
    return m_inputsMap.at(name);
}

const std::vector<AbstractData *> & Stage::outputs() const
{
    return m_outputs;
}

const AbstractData * Stage::output(const std::string & name) const
{
    return m_outputsMap.at(name);
}

void Stage::registerInput(AbstractInputSlot * input)
{
    // Check parameters
    if (!input) {
        return;
    }

    // Add input
    m_inputs.push_back(input);
    if (input->name() != "") {
        m_inputsMap.insert(std::make_pair(input->name(), input));        
    }

    // Emit signal
    inputAdded(input);
}

void Stage::unregisterInput(AbstractInputSlot * input)
{
    // Check parameters
    if (!input)
    {
        return;
    }

    // Find input
    auto it = std::find(m_inputs.begin(), m_inputs.end(), input);
    if (it != m_inputs.end())
    {
        // Remove input
        m_inputs.erase(it);
        m_inputsMap.erase(input->name());
        inputRemoved(input);
    }
}

void Stage::registerOutput(AbstractData * output)
{
    // Check parameters
    if (!output) {
        return;
    }

    // Add output
    m_outputs.push_back(output);
    if (output->name() != "") {
        m_outputsMap.insert(std::make_pair(output->name(), output));        
    }

    // Emit signal
    outputAdded(output);
}

void Stage::unregisterOutput(AbstractData * output)
{
    // Check parameters
    if (!output)
    {
        return;
    }

    // Find output
    auto it = std::find(m_outputs.begin(), m_outputs.end(), output);
    if (it != m_outputs.end())
    {
        // Remove output
        m_outputs.erase(it);
        m_outputsMap.erase(output->name());
        outputRemoved(output);
    }
}

void Stage::onContextInit(AbstractGLContext *)
{
}

void Stage::onContextDeinit(AbstractGLContext *)
{
}

void Stage::onProcess(AbstractGLContext *)
{
}


} // namespace gloperate
